#include "pch.h"
#include "XOInput.h"

//namespace xo
//{
//	XOInput::XOInput()
//	{
//	}
//
//	SelectionInfo XOInput::get_element_selection(XOViewTag current_view)
//	{
//		SelectionInfo selection_info;
//
//		// x, y, key = console.get_event();
//		unsigned x = 0, y = 0, key = 0;
//		if (key == -1) // nie wci�ni�to klawisza ==> to musia�a by� mysz
//		{
//
//		}
//		else
//		{
//			selection_info.first = true; // mysz klink�a ==> pressed == true
//
//			if (current_view == XOViewTag::game)
//			{
//				if (x > 0 && x < 50 && y > 0 && y < 50) // wcisnieto przycisk o numerze 1
//				{
//					selection_info.second = 1; // numer przycisku
//				}
//			}
//		}
//
//		return selection_info;
//	}
//}
