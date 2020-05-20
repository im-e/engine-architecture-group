#pragma once

/*! \file audioRollOff.h
\brief Provides RollOff definitions
*/

namespace Engine 
{
	/*! \enum RollOff
	\brief defines RollOFf
	*/
	enum class RollOff
	{
		Linear, //!< Linear rolloff
		LinearSquared, //!< Linear squared rolloff
		InverseTapered //!< InverseTapered rolloff
	};
}
