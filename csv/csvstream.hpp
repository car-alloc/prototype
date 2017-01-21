#ifndef __MODULE_CSV__
#define __MODULE_CSV__

#include <fstream>
#include <iomanip>
#include <string>

class csvstream
 {
  public:

      enum { max_field_length=(1<<16) };

  private:

      std::fstream f;
      std::ios_base::openmode mode;
      size_t current_line;
  
      bool first; // for commas
      bool text_bool; // accepts "true" "false" instead of 0 1
      bool empty_zero; // an empty numerical field is treated as zero
      bool trims; // trims whitespaces


      ////////////////////////
      std::string p_trim(const std::string & s) const;
      void p_put_comma();
      std::string p_encode(const std::string & s) const;
      std::string p_decode(const std::string & s) const;
      std::string p_get_next();
  
  public:

      /////////////////////////////////
      size_t where() const;
  
      bool eof() const;
      operator bool() const { return (bool)f; }
      void ws();
      void flush();
  
      void skip_to_eol();
  
      void text_as_bool(bool b);
      void empty_as_zero(bool z);
      void trim(bool t);

      bool is_text_as_bool() const { return text_bool; }
      bool is_empty_as_zero() const { return empty_zero; }
      bool is_trimmed() const { return trims; }

      //////////////////////////////////
      csvstream & operator<<(const std::string & s);
      csvstream & operator<<(const char * s);
      csvstream & operator>>(std::string & s);
      csvstream & operator>>(int & i);
      csvstream & operator>>(float & f);
      csvstream & operator>>(double & f);
      csvstream & operator>>(bool & b);

      //////////////////////////////////
      //
      // deals with everything else, provided
      // it has a conversion to string, or knows
      // how to << on a std::fstream
      //
      template <typename T>
      csvstream & operator<<(const T & x)
       {
        p_put_comma();
        f << x;

        return *this;
       }

      //////////////////////////////////
      //
      // deals with everything else, provided
      // it has a conv-ctor from string
      // 
      template <typename T>
      csvstream & operator>>(T & x)
       {
        x=T(p_decode(p_get_next()));
        return *this;
       }

  
      void close();

  csvstream(const std::string & filename,
            std::ios_base::openmode _mode = std::ios_base::in)
   : f(filename,_mode),
     mode(_mode),
     current_line(0),
     first(true),
     text_bool(true),
     empty_zero(true),
     trims(true)
   {}

  ~csvstream()
   {
    close();
   }

  // stubs for manipulators
  typedef csvstream & (*csvstream_manip)(csvstream &);
  csvstream & operator<<(csvstream_manip manip) { return manip(*this); }
  csvstream & operator>>(csvstream_manip manip) { return manip(*this); }
 };




////////////////////////////////////////
//
// Overload of endl and ws for csvstreams
//
namespace std
 {
  csvstream & endl(csvstream & out);
  csvstream & ws(csvstream & in);
 }

#endif
 //__MODULE_CSV__
