/* This file is part of VoltDB.
 * Copyright (C) 2008-2019 VoltDB Inc.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Affero General Public License as
 * published by the Free Software Foundation, either version 3 of the
 * License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Affero General Public License for more details.
 *
 * You should have received a copy of the GNU Affero General Public License
 * along with VoltDB.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef SERIALIZABLEEEEXCEPTION_H_
#define SERIALIZABLEEEEXCEPTION_H_

#include <string>
#include <stdexcept>

#define throwSerializableEEException(...) do { char msg[8192]; snprintf(msg, 8192, __VA_ARGS__); throw voltdb::SerializableEEException(VOLT_EE_EXCEPTION_TYPE_EEEXCEPTION, msg); } while (false)

namespace voltdb {

class ReferenceSerializeOutput;

enum VoltEEExceptionType {
    VOLT_EE_EXCEPTION_TYPE_NONE = 0,
    VOLT_EE_EXCEPTION_TYPE_EEEXCEPTION = 1,
    VOLT_EE_EXCEPTION_TYPE_SQL = 2,
    VOLT_EE_EXCEPTION_TYPE_CONSTRAINT_VIOLATION = 3,
    VOLT_EE_EXCEPTION_TYPE_INTERRUPT = 4,
    VOLT_EE_EXCEPTION_TYPE_TXN_RESTART = 5,
    VOLT_EE_EXCEPTION_TYPE_TXN_TERMINATION = 6,
    VOLT_EE_EXCEPTION_TYPE_SPECIFIED = 7,
    VOLT_EE_EXCEPTION_TYPE_GENERIC = 8,
    VOLT_EE_EXCEPTION_TYPE_TXN_MISPARTITIONED = 9,
    VOLT_EE_EXCEPTION_TYPE_REPLICATED_TABLE = 10,
    VOLT_EE_EXCEPTION_TYPE_DR_TABLE_NOT_FOUND = 11,
};

/*
 * An "Exception" that can be generated by the EE, serialized into the
 * exception buffer of the Engine, and deserialized on the Java side
 * and then thrown as a Java exception. This can end up being a
 * SQLException or an EEException depending on the exception type.
 *
 * The VoltEEExceptionType enum is synonymous with the
 * SerializableException.SerializableExceptions enum in Java and is
 * used to determine what exception class will be used to deserialize
 * this exception.
 */
class SerializableEEException : public std::runtime_error {
public:
    /*
     * Constructor that performs the serialization to the engines
     * exception buffer.
     */
    SerializableEEException(VoltEEExceptionType exceptionType, std::string message);
    SerializableEEException(std::string message);
    virtual ~SerializableEEException() throw();

    void serialize (ReferenceSerializeOutput *output) const;
    virtual const std::string message() const { return m_message; }
    VoltEEExceptionType getType() const { return m_exceptionType; }
    void appendContextToMessage(const std::string& more) { m_message += more; }
protected:
    virtual void p_serialize(ReferenceSerializeOutput *output) const {};

private:
    const VoltEEExceptionType m_exceptionType;
    std::string m_message;

};

class UnexpectedEEException : public SerializableEEException {
public:
    UnexpectedEEException(std::string message)
      : SerializableEEException(message)
    { }
};

} // end namespace voltdb
#endif /* SERIALIZABLEEEEXCEPTION_H_ */
