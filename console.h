//
// Nov-2021, Michael Lindner
// MIT license
//
#pragma once
#include <cassert>
#include <clocale>
#include <string>
#include <type_traits>
#include <vector>
#ifdef _WIN32
#include <windows.h>
#undef max
#endif


namespace internals
{
///////////////////

// RAII class for setting the global locale to UTF8 for the lifetime of
// an instance.
class Utf8Locale
{
 public:
   Utf8Locale();
   ~Utf8Locale();
   Utf8Locale(const Utf8Locale&) = delete;
   Utf8Locale(Utf8Locale&&) = default;
   Utf8Locale& operator=(const Utf8Locale&) = delete;
   Utf8Locale& operator=(Utf8Locale&&) = default;

 private:
   std::string m_prevLocale;
};


inline Utf8Locale::Utf8Locale()
{
   char* prevLocale = std::setlocale(LC_ALL, "en_US.utf8");
   if (prevLocale)
      m_prevLocale = prevLocale;
}

inline Utf8Locale::~Utf8Locale()
{
   if (!m_prevLocale.empty())
      std::setlocale(LC_ALL, m_prevLocale.c_str());
}


///////////////////

inline std::wstring utf16(const std::string& s)
{
   if (s.empty())
      return L"";

   Utf8Locale utf8Locale;

   std::vector<wchar_t> buffer(s.size() * 4, 0);
   const size_t numBytesWritten = mbstowcs(buffer.data(), s.c_str(), buffer.size());
   if (numBytesWritten == -1)
      return L"";

   return buffer.data();
}


///////////////////

inline std::wstring buildText()
{
   return {};
}


template <typename T> std::wstring buildText(const T& val)
{
   // wchar_t types.
   if constexpr (std::is_same_v<std::remove_const_t<std::remove_reference_t<T>>,
                                wchar_t*> ||
                 std::is_same_v<std::remove_const_t<std::remove_reference_t<T>>,
                                const wchar_t*>)
   {
      return val;
   }
   else if constexpr (std::is_same_v<T, std::wstring>)
   {
      return val;
   }
   // char types.
   else if constexpr (std::is_same_v<std::remove_const_t<std::remove_reference_t<T>>,
                                     char*> ||
                      std::is_same_v<std::remove_const_t<std::remove_reference_t<T>>,
                                     const char*>)
   {
      return utf16(val);
   }
   else if constexpr (std::is_same_v<T, std::string>)
   {
      return utf16(val);
   }
   // Other types.
   else if constexpr (std::is_same_v<T, bool>)
   {
      return val ? L"true" : L"false";
   }
   else if constexpr (std::is_enum_v<T>)
   {
      return std::to_wstring(static_cast<int>(val));
   }
   else if constexpr (std::is_integral_v<T> || std::is_floating_point_v<T>)
   {
      return std::to_wstring(val);
   }
   else
   {
#ifdef windows
      static_assert(false, "Console - Unknown type.");
#endif
      assert(false && "Console - Unknown type.");
      return {};
   }
}


template <typename T, typename... Args> std::wstring buildText(const T& val, Args... rest)
{
   return buildText(val) + L" " + buildText(rest...);
}

} // namespace internals


template <typename... Args> void consoleOut(Args... args)
{
#ifndef NDEBUG
   const std::wstring text = internals::buildText(args...) + L"\n";

#ifdef _WIN32
   OutputDebugStringW(text.c_str());
#else
   assert(false && "todo");
#endif // Platform

#endif // NDEBUG
}
