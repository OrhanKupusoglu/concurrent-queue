#include "../include/taxicab_number.h"
#include "../include/utility.h"

// by Howard Hinnant, modified for ms & C++11
// https://stackoverflow.com/questions/60046147/how-to-convert-chronoseconds-to-string-in-hhmmss-format-in-c
std::string Utility::elapsed_time(std::chrono::milliseconds msecs) {
    bool neg = msecs < std::chrono::seconds::zero();

    if (neg) {
        msecs = -msecs;
    }

    auto h = std::chrono::duration_cast<std::chrono::hours>(msecs);
    msecs -= h;
    auto m = std::chrono::duration_cast<std::chrono::minutes>(msecs);
    msecs -= m;
    auto s = std::chrono::duration_cast<std::chrono::seconds>(msecs);
    msecs -= s;

    std::string result;

    if (neg) {
        result += '-';
    }

    if (h < std::chrono::hours{10}) {
        result += '0';
    }

    result += std::to_string(h / std::chrono::hours{1});
    result += ':';

    if (m < std::chrono::minutes{10}) {
        result += '0';
    }

    result += std::to_string(m / std::chrono::minutes{1});
    result += ':';

    if (s < std::chrono::seconds{10}) {
        result += '0';
    }

    result += std::to_string(s / std::chrono::seconds{1});
    result += '.';

    if (msecs < std::chrono::milliseconds{10}) {
        result += "00";
    } else if (msecs < std::chrono::milliseconds{100}) {
        result += '0';
    }

    result += std::to_string(msecs / std::chrono::milliseconds{1});

    return result;
}

std::string Utility::gen_file_name() {
    std::ostringstream dump;
    std::time_t t = std::time(nullptr);
    std::tm tm = *std::localtime(&t);
    dump << std::put_time(&tm, "%Y-%m-%d_%H-%M-%S");

    return _base->_prefix + dump.str() + (_base->_json ? ".json" : ".txt");
}

void Utility::report_header(std::ostringstream& dump, const uint32_t rank, const std::size_t size_all, const std::size_t size_tc) {
    const int width = 36;
    dump << std::setw(width) << std::right << "taxicab range N = " << _base->_N << '\n'
         << std::setw(width) << std::right << "integer cube range R = " << _base->_R << '\n'
         << std::setw(width) << std::right << "number of worker threads T = " << _base->_T << '\n'
         << "\nall numbers with cubes [1, " << _base->_N << "]: " << size_all  << '\n'
         << "Taxicab(" << rank << ") or more in range [1, " << _base->_N << "]: " << size_tc << '\n';
}

void Utility::report_footer(std::ostringstream& dump) {
    dump << "elapsed time: " << elapsed_time(std::chrono::duration_cast<std::chrono::milliseconds>(_base->_t_end - _base->_t_start)) << '\n';
}

void Utility::report_meta(std::ostringstream& dump, const uint32_t rank, const std::size_t size_all, const std::size_t size_tc) {
    dump << "{\"meta\":{\"n\":" << _base->_N
         << ",\"r\":" << _base->_R
         << ",\"t\":" << _base->_T
         << ",\"total_cubes\":" << size_all
         << ",\"total_taxicabs\":" << size_tc
         << ",\"elapsed_time\":\"" << elapsed_time(std::chrono::duration_cast<std::chrono::milliseconds>(_base->_t_end - _base->_t_start)) << "\"},\n";
}

void Utility::dump_txt_taxicab_number(const int rank, const std::size_t size_all, const std::size_t size_tc) {
    std::ostringstream dump;
    report_header(dump, rank, size_all, size_tc);

    int i = 0;
    for (TaxiCab tc : _base->_taxicab) {
        dump << std::setw(5) << ++i << '.' << '\t'
             << std::setw(10) << static_cast<int64_t>(tc.taxicab_no);

        for (int j = 0; j < tc.cube.size(); j += 2) {
            dump << " = "
                 << std::setw(3) << tc.cube[j].first << "^3 + "
                 << std::setw(3) << tc.cube[j].second << "^3";
        }

        dump << '\n';
    }
    dump << '\n';

    report_footer(dump);
    save_report(dump);
}

void Utility::dump_json_taxicab_number(const int rank, const std::size_t size_all, const std::size_t size_tc) {
    std::ostringstream dump;
    report_meta(dump, rank, size_all, size_tc);

    dump << "\"list\":[\n";
    if (_base->_taxicab.size() > 0) {
        for (TaxiCab tc : _base->_taxicab) {
            dump << "{\"taxicab\":" << static_cast<int64_t>(tc.taxicab_no) << ",\"cubes\":[";

            for (int j = 0; j < tc.cube.size(); ++j) {
                dump << "[" << tc.cube[j].first << ", "
                     << tc.cube[j].second << "],";
            }
            dump.seekp(-1, std::ios_base::end);
            dump << "]},\n";
        }
        dump.seekp(-2, std::ios_base::end);
        dump << '\n';
    }
    dump << "]}\n";

    save_report(dump);
}

void Utility::save_report(std::ostringstream& dump) {
    std::string file_name = gen_file_name();

    if (_base->_display) {
        std::cout << _base->_output_dir + "/" + file_name << '\n';
    }

    save_to_txt_file(file_name, dump.str());
}

bool Utility::save_to_txt_file(const std::string& file_name, const std::string& content) {
    std::ofstream out_file;
    out_file.exceptions(std::ofstream::failbit | std::ofstream::badbit);

    try {
        out_file.open(_base->_output_dir + "/" + file_name, std::ios::out | std::ios::trunc);

        if (!out_file.is_open()) {
            std::cerr << "error while opening file '" << file_name << "'\n";
            return false;
        }

        out_file << content;
        out_file.close();

        return true;
    } catch (const std::ofstream::failure& e) {
        std::cerr << "exception while writing to file '" << file_name << "' | err: " << e.what() << '\n';
        return false;
    }
}

