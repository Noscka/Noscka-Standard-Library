#ifndef _STRING_NOSSTDLIB_HPP_
#define _STRING_NOSSTDLIB_HPP_

#include <string>
#include <stringapiset.h>
#include <vector>

namespace NosStdLib
{
	/// <summary>
		/// namespace for items which are related to strings (both string and wstring)
		/// </summary>
	namespace String
	{
		/// <summary>
		/// Converts string to wstring
		/// </summary>
		/// <param name="str">- string for converting</param>
		/// <returns>wstring version of the string</returns>
		std::wstring ToWstring(const std::string& str)
		{
			if (str.empty()) return std::wstring();
			int size_needed = MultiByteToWideChar(CP_UTF8, 0, &str[0], (int)str.size(), NULL, 0);
			std::wstring wstrTo(size_needed, 0);
			MultiByteToWideChar(CP_UTF8, 0, &str[0], (int)str.size(), &wstrTo[0], size_needed);
			return wstrTo;
		}

		/// <summary>
		/// Converts wstring to string
		/// </summary>
		/// <param name="wstr">- wstring for converting</param>
		/// <returns>string version of the string</returns>
		std::string ToString(const std::wstring& wstr)
		{
			if (wstr.empty()) return std::string();
			int size_needed = WideCharToMultiByte(CP_UTF8, 0, &wstr[0], (int)wstr.size(), NULL, 0, NULL, NULL);
			std::string strTo(size_needed, 0);
			WideCharToMultiByte(CP_UTF8, 0, &wstr[0], (int)wstr.size(), &strTo[0], size_needed, NULL, NULL);
			return strTo;
		}

	#pragma region IsNumber
		/// <summary>
		/// Check if wstring is number (with or without signs)
		/// </summary>
		/// <typeparam name="CharT">- string type</typeparam>
		/// <param name="str">- string to check</param>
		/// <param name="allowSigns">(default = true) - if it should allow signs at the start of the string (-123 or +123)</param>
		/// <returns>if string is valid number</returns>
		template <typename CharT>
		bool IsNumber(const std::basic_string<CharT>& str, bool allowSigns = true)
		{
			/* Iterator int, allows for changing start position */
			int Iteration = 0;
			if (allowSigns)
			{
				/* if allowSigns is true, check if first character is either - or + or a number */
				if ((str[0] != L'-' && str[0] != L'+') && !std::isdigit(str[0]))
					return false;

				Iteration = 1; /* Make Iterator go up 1 so for loop doesn't check first character */
			}

			/* Simple for loop, checking if any char isn't a digit */
			for (; Iteration < str.size(); Iteration++)
			{
				if (!std::isdigit(str[Iteration])) return false;
			}

			return true;
		}
	#pragma endregion

	#pragma region Split
		/// <summary>
		/// Split a string into a vectory array using a delimiter
		/// </summary>
		/// <typeparam name="CharT">- string type</typeparam>
		/// <param name="result">- the vector that will get modified</param>
		/// <param name="input">- the input that will get split</param>
		/// <param name="delimiter">(default = L' ') - delimiter which will determine the split</param>
		/// <returns>pointer to modified vector</returns>
		template <typename CharT>
		std::vector<std::basic_string<CharT>>* Split(std::vector<std::basic_string<CharT>>* result, std::basic_string<CharT>* input, const CharT& delimiter = L' ')
		{
			std::basic_string<CharT> tmp;
			std::basic_stringstream<CharT> ss(*input);

			while (getline(ss, tmp, delimiter))
			{
				result->push_back(tmp);
			}

			return result;
		}
	#pragma endregion

	#pragma region CenterString
		/// <summary>
		/// Center string with custom console Handle
		/// </summary>
		/// <typeparam name="CharT">- string type</typeparam>
		/// <param name="consoleHandle">- Custom Console Handle</param>
		/// <param name="input">- wstring to center</param>
		/// <param name="all">(default = true) - if it should center just first line or all lines</param>
		/// <returns>centered string</returns>
		template <typename CharT>
		std::basic_string<CharT> CenterString(HANDLE consoleHandle, std::basic_string<CharT> input, bool rightPadding = false, bool all = true)
		{
			CONSOLE_SCREEN_BUFFER_INFO csbi; /* TODO: change from using consoleHandle or add overload to use ConsoleSizeStruct */
			GetConsoleScreenBufferInfo(consoleHandle, &csbi);
			int columns = csbi.srWindow.Right - csbi.srWindow.Left + 1;
			if (all)
			{
				std::vector<std::basic_string<CharT>> inputSplit;
				std::basic_string<CharT> output;
				NosStdLib::String::Split<CharT>(&inputSplit, &input, L'\n');

				for (std::basic_string<CharT> Singleinput : inputSplit)
				{
					output += (std::basic_string<CharT>(((columns / 2) - Singleinput.size() / 2), ' ') + Singleinput + (rightPadding ? std::wstring(max((columns - (Singleinput.size() + ((columns / 2) - Singleinput.size() / 2))), 0), L' ') : L"") + (CharT)(L'\n'));
				}

				return output;
			}
			else
			{
				return (std::basic_string<CharT>(((columns / 2) - input.size() / 2), ' ') + input + (rightPadding ? std::wstring(max((columns - (input.size() + ((columns / 2) - input.size() / 2))), 0), L' ') : L"") + (CharT)(L'\n'));
			}
		}

		/// <summary>
		/// Center wstring 
		/// </summary>
		/// <typeparam name="CharT">- string type</typeparam>
		/// <param name="input">- wstring to center</param>
		/// <param name="all">(default = true) - if it should center just first line or all lines</param>
		/// <returns>centered wstring</returns>
		template <typename CharT>
		std::basic_string<CharT> CenterString(std::basic_string<CharT> input, bool rightPadding = false, bool all = true)
		{
			return CenterString<CharT>(GetStdHandle(STD_OUTPUT_HANDLE), input, rightPadding, all);
		}
	#pragma endregion
	}
}
#endif