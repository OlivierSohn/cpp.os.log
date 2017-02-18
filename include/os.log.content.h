

namespace imajuscule {
    void logWithLineNumbers(std::string const &);

    template<typename T>
    void logRange(range<T> r) {
        if(r.empty()) {
            LG(INFO, "empty");
        }
        else {
            LG(INFO, "%f .. %f", r.getMin(), r.getMax());
        }
    }
}
