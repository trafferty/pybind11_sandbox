class Matrix {
public:
    Matrix(size_t rows, size_t cols) : m_rows(rows), m_cols(cols) {
        std::cout << "Value constructor: Creating a " << rows << "x" << cols << " matrix " << std::endl;
        m_data = new unsigned short[rows*cols];
        memset(m_data, 0, sizeof(unsigned short) * rows * cols);
    }

    Matrix(const Matrix &s) : m_rows(s.m_rows), m_cols(s.m_cols) {
        std::cout << "Copy constructor: Creating a " << m_rows << "x" << m_cols << " matrix " << std::endl;
        m_data = new unsigned short[m_rows * m_cols];
        memcpy(m_data, s.m_data, sizeof(unsigned short) * m_rows * m_cols);
    }

    Matrix(Matrix &&s) : m_rows(s.m_rows), m_cols(s.m_cols), m_data(s.m_data) {
        std::cout << "Move constructor: Creating a " << m_rows << "x" << m_cols << " matrix " << std::endl;
        s.m_rows = 0;
        s.m_cols = 0;
        s.m_data = nullptr;
    }

    ~Matrix() {
        std::cout << "Freeing a " << m_rows << "x" << m_cols << " matrix " << std::endl;
        delete[] m_data;
    }

    Matrix &operator=(const Matrix &s) {
        std::cout << "Assignment operator : Creating a " << s.m_rows << "x" << s.m_cols << " matrix " << std::endl;
        delete[] m_data;
        m_rows = s.m_rows;
        m_cols = s.m_cols;
        m_data = new unsigned short[m_rows * m_cols];
        memcpy(m_data, s.m_data, sizeof(unsigned short) * m_rows * m_cols);
        return *this;
    }

    Matrix &operator=(Matrix &&s) {
        std::cout << "Move assignment operator : Creating a " << s.m_rows << "x" << s.m_cols << " matrix " << std::endl;
        if (&s != this) {
            delete[] m_data;
            m_rows = s.m_rows; m_cols = s.m_cols; m_data = s.m_data;
            s.m_rows = 0; s.m_cols = 0; s.m_data = nullptr;
        }
        return *this;
    }

    unsigned short operator()(size_t i, size_t j) const {
        return m_data[i*m_cols + j];
    }

    unsigned short &operator()(size_t i, size_t j) {
        return m_data[i*m_cols + j];
    }

    unsigned short *data() { return m_data; }

    size_t rows() const { return m_rows; }
    size_t cols() const { return m_cols; }
private:
    size_t m_rows;
    size_t m_cols;
    unsigned short *m_data;
};
