/*
 * Project: class "aini" (C++ binding for "ini_t")
 * Version: 0.2b
 * File: "aini.h"
 * (C) 2007-2015 Alex Zorg <azorg@mail.ru>
 */

#ifndef AINI_H
#define AINI_H
//----------------------------------------------------------------------------
#include <string> // std::string
#include "ini.h"  // ini_t/str_t
//----------------------------------------------------------------------------
namespace aclass {
//----------------------------------------------------------------------------
class aini
{
private:
  ini_t f;

public:
  // constructor and set file name
  inline aini(const char *fname) { ini_init_fname(&f, fname); }
  inline aini(const std::string &fname) { ini_init_fname(&f, fname.c_str()); }

  // default constructor
  inline aini() { ini_init_fname(&f, ""); }

  // destructor
  inline virtual ~aini() { ini_free(&f); }

  // save to file if open for write or always (if save OK return true)
  inline bool save(bool always_save=false)
  { return !!ini_save_ex(&f, always_save); }

  // get/set state
  inline ini_state_t get_state() const { return ini_get_state(&f); }
  inline void set_state(ini_state_t st) { ini_set_state(&f, st); }

  // load from C-string/std::string
  inline void load(const char *text) { ini_load(&f, text); }
  inline void load(const std::string &text) { load(text.c_str()); }

  // return as pointer to C-string
  inline const char *cstr() const { return ini_cstr(&f); }

  // return file name as C-string
  inline const char *get_fname() const { return ini_get_fname(&f); }

  // set file name
  inline void set_fname(const char *fname) { ini_set_fname(&f, fname); }
  inline void set_fname(const std::string &fname)
  { set_fname(fname.c_str()); }

  // check section
  bool has_section(const char *section)
  { return !!ini_has_section(&f, section); }
  bool has_section(const std::string &section)
  { return !!ini_has_section(&f, section.c_str()); }

  // check ident
  bool has_ident(const char *section, const char *ident)
  { return !!ini_has_ident(&f, section, ident); }
  bool has_ident(const std::string &section, const std::string &ident)
  { return !!ini_has_ident(&f, section.c_str(), ident.c_str()); }

  // read value "as-is"
  inline std::string read_value(const char *section,
                                const char *ident,
                                const char *default_value)
  {
    str_t val = ini_read_value(&f, section, ident, default_value);
    std::string ret_val = str_c(&val);
    str_free(&val);
    return ret_val;
  }
  inline std::string read_value(const std::string &section,
                                const std::string &ident,
                                const std::string &default_value)
  {
    str_t val = ini_read_value(&f, section.c_str(), ident.c_str(),
                               default_value.c_str());
    std::string ret_val = str_c(&val);
    str_free(&val);
    return ret_val;
  }

  // write value "as-is"
  inline void write_value(const char *section,
                          const char *ident,
                          const char *value)
  {
    ini_write_value(&f, section, ident, value);
  }
  inline void write_value(const std::string &section,
                          const std::string &ident,
                          const std::string &value)
  {
    ini_write_value(&f, section.c_str(), ident.c_str(),
                    value.c_str());
  }

  // code string to write to INI-file
  inline static std::string code_str(const char *str)
  {
    str_t val = ini_code_str(str);
    std::string ret_val(str_c(&val), str_size(&val));
    str_free(&val);
    return ret_val;
  }
  inline static std::string code_str(const std::string &str)
  { return aini::code_str(str.c_str()); }

  // decode string to read from INI-file
  inline static std::string decode_str(const char *str)
  {
    str_t val = ini_decode_str(str);
    std::string ret_val(str_c(&val), str_size(&val));
    str_free(&val);
    return ret_val;
  }
  inline static std::string decode_str(const std::string &str)
  { return aini::decode_str(str.c_str()); }

  // read value as string
  inline std::string read_str(const char *section,
                              const char *ident,
                              const char *default_value)
  {
    str_t val = ini_read_str(&f, section, ident, default_value);
    std::string ret_val(str_c(&val), str_size(&val));
    str_free(&val);
    return ret_val;
  }
  inline std::string read_str(const std::string &section,
                              const std::string &ident,
                              const std::string &default_value)
  {
    str_t val = ini_read_str(&f, section.c_str(), ident.c_str(),
                             default_value.c_str());
    std::string ret_val = str_c(&val);
    str_free(&val);
    return ret_val;
  }

  // write value as string
  inline void write_str(const char *section,
                        const char *ident,
                        const char *value)
  {
    ini_write_str(&f, section, ident, value);
  }
  inline void write_str(const std::string &section,
                        const std::string &ident,
                        const std::string &value)
  {
    ini_write_str(&f, section.c_str(), ident.c_str(), value.c_str());
  }

#ifdef STR_EXTRA
  // read value as byte array
  inline std::string read_bin(const char *section,
                              const char *ident,
                              const char *default_value,
                              int default_size)
  {
    str_t val = ini_read_bin(&f, section, ident,
                             default_value, default_size);
    std::string ret_val(str_c(&val), str_size(&val));
    str_free(&val);
    return ret_val;
  }
  inline std::string read_bin(const std::string &section,
                              const std::string &ident,
                              const std::string &default_value)
  {
    str_t val = ini_read_bin(&f, section.c_str(), ident.c_str(),
                             default_value.c_str(),
                             default_value.size());
    std::string ret_val(str_c(&val), str_size(&val));
    str_free(&val);
    return ret_val;
  }
  
  // write value as byte array
  inline void write_bin(const char *section,
                        const char *ident,
                        const char *value, int size)
  {
    ini_write_bin(&f, section, ident, value, size);
  }
  inline void write_bin(const std::string &section,
                        const std::string &ident,
                        const std::string &value)
  {
    ini_write_bin(&f, section.c_str(), ident.c_str(),
                  value.c_str(), value.size());
  }
#endif // STR_EXTRA

  // read value as long
  inline long read_long(const char *section,
                        const char *ident,
                        long default_value)
  {
    return ini_read_long(&f, section, ident, default_value);
  }
  inline long read_long(const std::string &section,
                        const std::string &ident,
                        long default_value)
  {
    return ini_read_long(&f, section.c_str(), ident.c_str(), default_value);
  }

  // write value as long
  inline void write_long(const char *section,
                         const char *ident,
                         long value)
  {
    ini_write_long(&f, section, ident, value);
  }
  inline void write_long(const std::string &section,
                         const std::string &ident,
                         long value)
  {
    ini_write_long(&f, section.c_str(), ident.c_str(), value);
  }

#ifdef STR_INT64_DEF
  // read value as int64 (HEX format)
  inline STR_INT64 read_int64(const char *section,
                              const char *ident,
                              STR_INT64 default_value)
  {
    return ini_read_int64(&f, section, ident, default_value);
  }
  inline STR_INT64 read_int64(const std::string &section,
                              const std::string &ident,
                              STR_INT64 default_value)
  {
    return ini_read_int64(&f, section.c_str(), ident.c_str(), default_value);
  }
  
  // write value as int64 (HEX format)
  inline void write_int64(const char *section,
                          const char *ident,
                          STR_INT64 value)
  {
    ini_write_int64(&f, section, ident, value);
  }
  inline void write_int64(const std::string &section,
                          const std::string &ident,
                          STR_INT64 value)
  {
    ini_write_int64(&f, section.c_str(), ident.c_str(), value);
  }
#endif // STR_INT64_DEF

#ifdef STR_FLOAT
  // read value as float
  inline float read_float(const char *section,
                          const char *ident,
                          float default_value)
  {
    return ini_read_float(&f, section, ident, default_value);
  }
  inline float read_float(const std::string &section,
                          const std::string &ident,
                          float default_value)
  {
    return ini_read_float(&f, section.c_str(), ident.c_str(), default_value);
  }
  
  // write value as float
  inline void write_float(const char *section,
                          const char *ident,
                          float value)
  {
    ini_write_float(&f, section, ident, value);
  }
  inline void write_float(const std::string &section,
                          const std::string &ident,
                          float value)
  {
    ini_write_float(&f, section.c_str(), ident.c_str(), value);
  }

  // read value as double
  inline float read_double(const char *section,
                           const char *ident,
                           double default_value)
  {
    return (float) ini_read_double(&f, section, ident, default_value);
  }
  inline float read_double(const std::string &section,
                           const std::string &ident,
                           double default_value)
  {
    return (float) ini_read_double(&f, section.c_str(), ident.c_str(), default_value);
  }
  
  // write value as double
  inline void write_double(const char *section,
                           const char *ident,
                           double value)
  {
    ini_write_double(&f, section, ident, value);
  }
  inline void write_double(const std::string &section,
                           const std::string &ident,
                           double value)
  {
    ini_write_double(&f, section.c_str(), ident.c_str(), value);
  }
#endif // STR_FLOAT

  // read value as bool
  inline bool read_bool(const char *section,
                        const char *ident,
                        bool default_value)
  {
    return !!ini_read_bool(&f, section, ident, default_value);
  }
  inline bool read_bool(const std::string &section,
                        const std::string &ident,
                        bool default_value)
  {
    return !!ini_read_bool(&f, section.c_str(), ident.c_str(), default_value);
  }

  // write value as bool
  inline void write_bool(const char *section,
                         const char *ident,
                         bool value)
  {
    ini_write_bool(&f, section, ident, value);
  }
  inline void write_bool(const std::string &section,
                         const std::string &ident,
                         bool value)
  {
    ini_write_bool(&f, section.c_str(), ident.c_str(), value);
  }

};
//----------------------------------------------------------------------------
} // namespace aclass
//----------------------------------------------------------------------------
#endif // AINI_H

/*** end of "aini.h" ***/

