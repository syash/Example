typedef std::vector<var> varVector;

typedef std::pair <std::string,
                   std::vector<std::string> > operPair;


typedef std::vector<operPair> operVector;



struct ConstructVector : boost::static_visitor<void>
{
    ConstructVector(operVector& vec_, varVector& vs) : _vec(vec_),
                                                       _vs(vs) {}
    operVector& _vec;
    varVector&  _vs;

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
        for (stIter=vec.begin(); stIter != vec.end(); ++stIter) {

           boost::apply_visitor(*this, *stIter);
        }
        _vec.push_back(std::make_pair(op, _vs));
        _vs.clear();
    }

    void operator()(const unop<op_not>& u) const
    {
        boost::apply_visitor(*this, u.operand);
        _vec.push_back(std::make_pair("NOT", _vs));

       _vs.clear();
    }
};
