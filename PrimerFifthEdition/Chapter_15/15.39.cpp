#include "TextQuery.hpp"
#include <algorithm>
#include <fstream>

class Query_base
{
    friend class Query;

protected:
    using line_no = TextQuery::line_no;
    virtual ~Query_base() = default;

private:
    virtual QueryResult eval(const TextQuery&) const = 0;
    virtual std::string rep() const = 0;
};

class Query
{
    friend Query operator~(const Query&);
    friend Query operator|(const Query&, const Query&);
    friend Query operator&(const Query&, const Query&);

public:
    Query(const std::string&);

    QueryResult eval(const TextQuery& t) const
    {
        return q->eval(t);
    }

    std::string rep() const
    {
        return q->rep();
    }

private:
    Query(const std::shared_ptr<Query_base>& query) : q(query)
    {
    }

    std::shared_ptr<Query_base> q;
};

std::ostream& operator<<(std::ostream& os, const Query& query)
{
    return os << query.rep();
}

class WordQuery : public Query_base
{
    friend class Query;

    WordQuery(const std::string& s) : query_word(s)
    {
    }

    QueryResult eval(const TextQuery& t) const
    {
        return t.query(query_word);
    }

    std::string rep() const
    {
        return query_word;
    }

    std::string query_word;
};

inline Query::Query(const std::string& s) : q(new WordQuery(s))
{
}

class NotQuery : public Query_base
{
    friend Query operator~(const Query&);

    NotQuery(const Query& q) : query(q)
    {
    }

    std::string rep() const
    {
        return "~(" + query.rep() + ");";
    }

    QueryResult eval(const TextQuery& text) const
    {
        auto result = query.eval(text);
        auto ret_lines = std::make_shared<std::set<line_no>>();
        auto beg = result.begin(), end = result.end();
        auto sz = result.get_file()->size();
        for (size_t n = 0; n != sz; ++n)
        {
            if (beg == end || *beg != n)
            {
                ret_lines->insert(n);
            }
            else if (beg != end)
            {
                ++beg;
            }
        }

        return QueryResult(rep(), ret_lines, result.get_file());
    }

    Query query;
};

inline Query operator~(const Query& operand)
{
    return std::shared_ptr<Query_base>(new NotQuery(operand));
}

class BinaryQuery : public Query_base
{
protected:
    BinaryQuery(const Query& l, const Query& r, const std::string& s) : lhs(l), rhs(r), opSym(s)
    {
    }

    std::string rep() const
    {
        return '(' + lhs.rep() + ' ' + opSym + ' ' + rhs.rep() + ')';
    }

    Query lhs, rhs;    // 左侧右侧运算对象
    std::string opSym; // 运算符的名字
};

class AndQuery : public BinaryQuery
{
    friend Query operator&(const Query&, const Query&);

    AndQuery(const Query& left, const Query& right) : BinaryQuery(left, right, "&")
    {
    }

    QueryResult eval(const TextQuery& text) const
    {
        auto left = lhs.eval(text), right = rhs.eval(text);
        auto ret_lines = std::make_shared<std::set<line_no>>();
        std::set_intersection(left.begin(),
                              left.end(),
                              right.begin(),
                              right.end(),
                              std::inserter(*ret_lines, ret_lines->begin()));

        return QueryResult(rep(), ret_lines, left.get_file());
    }
};

inline Query operator&(const Query& lhs, const Query& rhs)
{
    return std::shared_ptr<Query_base>(new AndQuery(lhs, rhs));
}

class OrQuery : public BinaryQuery
{
    friend Query operator|(const Query&, const Query&);

    OrQuery(const Query& left, const Query& right) : BinaryQuery(left, right, "|")
    {
    }

    QueryResult eval(const TextQuery& text) const
    {
        auto right = rhs.eval(text), left = lhs.eval(text);
        auto ret_lines = std::make_shared<std::set<line_no>>(left.begin(), left.end());
        ret_lines->insert(right.begin(), right.end());

        return QueryResult(rep(), ret_lines, left.get_file());
    }
};

inline Query operator|(const Query& lhs, const Query& rhs)
{
    return std::shared_ptr<Query_base>(new OrQuery(lhs, rhs));
}

int main()
{
    Query q = Query("is") & Query("and");
    std::ifstream fin("Text.txt");

    print(std::cout, q.eval(TextQuery(fin)));

    return 0;
}
