struct operInfo
{
  std::string oper;
  std::vector<std::string> flags;
  int        precedence;
};

typedef std::vector<var> varVector;
typedef std::vector<operInfo> operVector;

struct ConstructVector : boost::static_visitor<void>
{
    ConstructVector(operVector& vec_,
                    varVector&  vs,
                    int&        precedence): _vec(vec_),
                                             _vs(vs),
                                             _precedence(precedence) {}
    operVector& _vec;
    varVector&  _vs;
    int&        _precedence;

    void operator()(const var& v) const {
       _vs.push_back(v);
    }

    void operator()(const combination_op<op_and>& b) const {
       Insert("AND", b.operands);
    }
    void operator()(const combination_op<op_or >& b) const {
       Insert("OR", b.operands);
    }

    void Insert(const std::string& op, const std::vector<expr>& vec) const
    {
        std::vector<expr>::const_iterator stIter, enditer;

        ++ _precedence;
        for (stIter=vec.begin(); stIter != vec.end(); ++stIter) {

           boost::apply_visitor(*this, *stIter);
        }

        operInfo oi;
        oi.oper  = op;
        oi.flags = (_vs);
        oi.precedence =  _precedence;
        _vec.push_back(oi);

        _vec.push_back(oi);
        _vs.clear();
        -- _precedence;
    }

    void operator()(const unop<op_not>& u) const
    {
        ++ _precedence;

        boost::apply_visitor(*this, u.operand);

        operInfo oi;
        oi.oper  = "NOT";
        oi.flags = _vs;
        oi.precedence = _precedence;

        _vec.push_back(oi);
        _vs.clear();
        -- _precedence;

    }
};
