
Error1D::Error1D() {  }
Error1D::Error1D(double symm_err) {  }
Error1D::Error1D(double minus_err, double plus_err) {  }

double Error1D::minusErr() const {  }
double Error1D::plusErr() const {  }
double Error1D::symmErr() const {  }
std::pair<double,double> Error1D::errs() const {  }

Error1D& Error1D::setMinusErr(double minus_err) {  }
Error1D& Error1D::setPlusErr(double plus_err) {  }
Error1D& Error1D::setErrs(double symm_err) {  }
Error1D& Error1D::setErrs(std::pair<double,double> pm_errs) {  }
Error1D& Error1D::setErrs(double minus_err, double plus_err) {  }


////////////////////////////////////////////////////


PointError::PointError(ErrorType type, const std::string& ann) {  }

PointError::PointError(std::vector<size_t, Error1D> err1Ds, ErrorType type=ERR_STAT, const std::string& ann) {  }

PointError::PointError(size_t dim, Error1D, ErrorType type=ERR_STAT, const std::string& ann) {  }

PointError::PointError(size_t dim, double symm_err, ErrorType type=ERR_STAT, const std::string& ann) {  }

PointError::PointError(size_t dim, std::pair<double,double> pm_errs, ErrorType type, const std::string& ann) {  }

PointError::PointError(size_t dim, double minus_err, double plus_err, ErrorType type, const std::string& ann) {  }

Error1D PointError::error(size_t dim) const {  }
double PointError::plusErr(size_t dim) const {  }
double PointError::minusErr(size_t dim) const {  }
double PointError::symmErr(size_t dim) const {  }
ErrorType PointError::type() const {  }
std::string PointError::annotation() const {  }

PointError& PointError::setError(size_t dim, const Error1D& err1D) {  }
PointError& PointError::setError(size_t dim, double symm_err) {  }
PointError& PointError::setError(size_t dim, std::pair<double,double> pm_errs) {  }
PointError& PointError::setError(size_t dim, double minus_err, double plus_err) {  }

PointError& PointError::setErrorClass(ErrorType type) {  }
PointError& PointError::setAnnotation(const std::string& ann) {  }
