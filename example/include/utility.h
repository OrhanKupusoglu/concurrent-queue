#ifndef UTILITY_H
#define UTILITY_H

class Base; // forward declaration

class Utility {
    public:
        Utility() = delete; // default constructor
        Utility(Base* base) : _base{base} {}

        std::string elapsed_time(std::chrono::milliseconds msecs);
        std::string gen_file_name();
        void report_header(std::ostringstream& dump, const uint32_t rank, const std::size_t size_all, const std::size_t size_tc);
        void report_footer(std::ostringstream& dump);
        void report_meta(std::ostringstream& dump, const uint32_t rank, const std::size_t size_all, const std::size_t size_tc);
        void dump_txt_taxicab_number(const int rank, const std::size_t size_all, const std::size_t size_tc);
        void dump_json_taxicab_number(const int rank, const std::size_t size_all, const std::size_t size_tc);
        void save_report(std::ostringstream& dump);
        bool save_to_txt_file(const std::string& file_name, const std::string& content);

    private:
        Base* _base;
};

#endif
