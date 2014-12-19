

#include <utl_profile_pass.h>


utl::ProfilePass::ProfilePass(const std::string& str, const utl::Dim& start, const utl::Dim& step, const utl::Dim& end, size_t __iter) :
	_name(str), _start(start), _step(step), _end(end), _iter(__iter), _print_n(true), _print_t(true), _print_o(true), _print_p(true) , _countUp(true)
{}

utl::ProfilePass::ProfilePass(const ProfilePass& p) :
	_name(p._name), _start(p._start), _step(p._step), _end(p._end), _iter(p._iter), _print_n(p._print_n), _print_t(p._print_t), _print_o(p._print_o), _print_p(p._print_p) , _countUp(p._countUp)
{
}

void utl::ProfilePass::run()
{
	//size_t j = 0;

	auto compare = _countUp ?
				[](const Dim& lhs, const Dim& rhs){ return (lhs < 1) == 0 && lhs <= rhs; } :
	[](const Dim& lhs, const Dim& rhs){ return (lhs < 1) == 0 && lhs >= rhs; };

	auto advance = _countUp ?
				[](Dim& lhs, const Dim& rhs){ lhs += rhs; } :
	[](Dim& lhs, const Dim& rhs){ lhs -= rhs; } ;

	for(Dim i = _start; compare(i, _end); advance(i,_step))
	{
		//			TRUE_COMMENT("start : " << this->_start.toString() << ", _end : " << this->_end.toString() << ", _step = " << this->_step.toString() << ", i " << i.toString() << ", comp = "  << compare(i,_end));

		Seconds time = this->prof(i);
		double op = this->ops(i);
		double perf = double(op)  / double(time.count());

		this->_elems.push_back(i.prod());
		this->_times.push_back(time) ;
		this->_ops.push_back(op) ; // 2 * n^2 + n
		this->_perf.push_back(perf);
	}

}


const std::vector<double> &
utl::ProfilePass::dims()    const { return _elems; }

const std::vector<utl::Seconds>&
utl::ProfilePass::times()   const { return _times; }

const std::vector<double> &
utl::ProfilePass::ops()     const { return _ops; }

const std::vector<double> &
utl::ProfilePass::perf()    const { return _perf; }

const std::string &
utl::ProfilePass::name()  const { return _name; }

void utl::ProfilePass::setCountUp()   { this->_countUp = true; }
void utl::ProfilePass::setCountDown() { this->_countUp = false; }

bool utl::ProfilePass::printNumber() const { return this->_print_n; }
bool utl::ProfilePass::printTime() const { return this->_print_t; }
bool utl::ProfilePass::printOperation() const { return this->_print_o; }
bool utl::ProfilePass::printPerformance() const { return this->_print_p; }

std::string
utl::ProfilePass::toString(const std::vector<utl::Seconds>& v) const
{
	if(v.empty()) return "[];";
	std::ostringstream oss;
	oss << '[';
	for ( size_t i = 0, size = v.size()-1u; i < size; ++i )
	{
		oss << v.at(i) << ',';
	}
	oss << v.back() << "];";
	return oss.str();
}

std::string
utl::ProfilePass::toString(const std::vector<double>& v) const
{
	if(v.empty()) return "[];";
	std::ostringstream oss;
	oss << '[';
	for ( size_t i = 0, size = v.size()-1u; i < size; ++i )
	{
		oss << v.at(i) << ',';
	}
	oss << v.back() << "];";
	return oss.str();
}

std::ostream& operator <<(std::ostream &out, const utl::ProfilePass& pass)
{

	out.precision(5);
	out << std::scientific;
	if(pass.printNumber())       out << pass.name() << "_n = " << pass.toString(pass.dims()) << std::endl;
	if(pass.printOperation())    out << pass.name() << "_o = " << pass.toString(pass.ops())   << std::endl;
	if(pass.printTime())         out << pass.name() << "_t = " << pass.toString(pass.times()) << std::endl;
	if(pass.printPerformance())  out << pass.name() << "_p = " << pass.toString(pass.perf())  << std::endl;
	return out;
}

