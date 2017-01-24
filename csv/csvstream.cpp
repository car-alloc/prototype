#include <string>
#include <csvstream.hpp>

#include <iostream>

namespace std
 {
  csvstream & endl(csvstream & out) { out.flush(); return out; }
  csvstream & ws(csvstream & in) { in.ws(); return in; }
 }

/////////////////////////////////////////
size_t csvstream::where() const { return current_line; }
  
bool csvstream::eof() const { return f.eof(); }
void csvstream::ws() { f >> std::ws; current_line++; first=true; }
void csvstream::flush() { f << std::endl; first=true; }
  
void csvstream::skip_to_eol() { while (f.get()!='\n'); }
  
void csvstream::text_as_bool(bool b) { text_bool=b; }
void csvstream::empty_as_zero(bool z) { empty_zero=z; }
void csvstream::trim(bool t) { trims=t; }

////////////////////////////////////////
std::string csvstream::p_trim(const std::string & s) const
 {
  if (trims)
   {
    std::string t(s);
    while ((t.size()) && (t.front()==' ')) t.erase(0,1);
    while ((t.size()) && (t.back()==' ')) t.pop_back();
    return t;
   }
  else
   return s;
 }

////////////////////////////////////////
void csvstream::p_put_comma() { if (!first) f << ','; first=false; }

///////////////////////////////////////
std::string csvstream::p_encode(const std::string & s) const
 {
  size_t quotes=s.find('"');
  size_t commas=s.find(',');
  size_t newlines=s.find('\n');

  if ( (quotes!=std::string::npos) ||
       (newlines!=std::string::npos) ||
       (commas!=std::string::npos) )
   {
    if (quotes!=std::string::npos)
     {
      std::string t(s);

      // replaces " by "" everywhere
      do
       {
        t.insert(quotes,1,'"');
        quotes=t.find('"',quotes+2);
       }
      while (quotes!=std::string::npos);

      // ...and wraps everything with
      // quotes on top
      return '"'+t+'"';
     }
    else
     // has only , or \n
     return '"'+s+'"';
   }
  else
   // has none of " , \n, so no
   // need to quote
   return s;
 }

////////////////////////////////////////
std::string csvstream::p_decode(const std::string & s) const
 {
  size_t quotes=s.find("\"\"");

  if (quotes)
   {
    std::string t(s);

    // replaces "" by " everywhere
    while (quotes!=std::string::npos)
     {
      t.erase(quotes,1);
      quotes=t.find("\"\"", quotes+1);
     }

    if ((t.front()=='"') && (t.back()=='"'))
     return p_trim(t.substr(1,t.length()-2));
    else
     return p_trim(t);
   }
  else
   return p_trim(s);
 }

////////////////////////////////////////
//
// gets the next field, upto the
// next comma, newline, or eof.
//
std::string csvstream::p_get_next()
 {
  size_t length=0,quotes=0;
  std::fstream::streampos start=f.tellg();
  bool comma=false,
   eol=false,
   eof=false;
  do
   {
    comma=eol=eof=false;

    length++;

    switch (f.get())
     {
     case '"':quotes++; break;
     case ',':comma=true; break;
     case '\r':
     case '\n': eol=true; break;
     case std::char_traits<char>::eof(): eof=true; break;

     default: break;
     }
   }
  while (!(
           (( comma || eol) && ((quotes & 1)==0))
           || (eof)
           || (length>max_field_length)
           )
         );

  if (eof)
   throw std::runtime_error("unexpected end of file");

  // if quotes are mismatched, or field
  // completely empty, then the syntax is
  // invalid
  if ((quotes & 1) || (length==0))
   throw std::runtime_error("invalid syntax");
  else
   if (length<=max_field_length)
    {
     std::string t(length,'\0');

     // rewinds to the start of the field
     f.seekg(start);

     // reads until , or eol
     for (size_t i=0;i<length;i++)
      t[i]=f.get();

     // if trailing , or eol (and therefore \n)
     // remove it
     if (comma||eol) t.pop_back();

     return t;
    }
   else
    throw std::runtime_error("field too long");
 }

////////////////////////////////////////
csvstream & csvstream::operator<<(const std::string & s)
 {
  p_put_comma();
  f << p_encode(s);
  return *this;
 }

////////////////////////////////////////
csvstream & csvstream::operator<<(const char * s)
 {
  return *this << std::string(s);
 }

////////////////////////////////////////
csvstream & csvstream::operator>>(std::string & s)
 {
  s=p_decode(p_get_next());
  return *this;
 }

////////////////////////////////////////
csvstream & csvstream::operator>>(int & i)
 {
  if (empty_zero)
   {
    std::string t=p_decode(p_get_next());
    if (t.size()==0)
     i=0;
    else i=std::stoi(t);
   }
  else
   i=std::stoi(p_decode(p_get_next()));

  return *this;
 }

////////////////////////////////////////
csvstream & csvstream::operator>>(float & f)
 {
  if (empty_zero)
   {
    std::string t=p_decode(p_get_next());
    if (t.size()==0)
     f=0;
    else
     f=std::stof(t);
   }
  else
   f=std::stof(p_decode(p_get_next()));
  return *this;
 }

////////////////////////////////////////
csvstream & csvstream::operator>>(double & f)
 {
  f=std::stod(p_decode(p_get_next()));
  return *this;
 }

////////////////////////////////////////
csvstream & csvstream::operator>>(bool & b)
 {
  if (text_bool)
   {
    std::string t=p_decode(p_get_next());
    for(char & c:t) c=std::tolower(c);
    if (t=="true")
     b=true;
    else
     if (t=="false")
      b=false;
     else
      if (empty_zero && (t.size()==0))
       b=false;
      else
       throw std::runtime_error("bool neither true nor false");
   }
  else
   b=std::stoi(p_decode(p_get_next()));
  return *this;
 }

////////////////////////////////////////
void csvstream::close()
 {
  if (mode==std::ios_base::out)
   {
    flush(); // CSV flush
    f.flush(); // fstream flush
   }

  f.close();
 }
