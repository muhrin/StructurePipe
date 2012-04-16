/*
 * IFinishNotifiable.h
 *
 *
 *  Created on: Feb 17, 2012
 *      Author: Martin Uhrin
 */

#ifndef I_FINISH_NOTIFIABLE_H
#define I_FINISH_NOTIFIABLE_H

// INCLUDES /////////////////////////////////////////////

// FORWARD DECLARATIONS ////////////////////////////////////

template <class DataType>
class IFinishNotifiable
{
public:

	virtual ~IFinishNotifiable() {}

	virtual void dataFinished(DataType & data) = 0;
};

#endif /* I_FINISH_NOTIFIABLE_H */
