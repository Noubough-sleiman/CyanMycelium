#ifndef _CM_PROTOBUF_PULLPARSER__
#define _CM_PROTOBUF_PULLPARSER__

#include <cstdint>
#include <string.h>

namespace CyanMycelium
{
    typedef enum : uint8_t
    {
        TT_BEGIN_MESSAGE = 0,
        TT_BEGIN_FIELD,
        TT_TAG,
        TT_VALUE,
        TT_END_MESSAGE,
    } pb_token_type_t;

    typedef enum : uint8_t
    {
        WT_VARINT = 0,
        WT_64BIT = 1,
        WT_LENGTH = 2,
        WT_32BIT = 5
    } pb_wire_type_t;

    typedef struct
    {
        uint8_t number;
        pb_wire_type_t type;
    } pb_tag;

    typedef struct
    {
        size_t byteConsumed;
        uint8_t currentDepth;
        size_t position;
        pb_token_type_t tokenType;
        pb_tag tag;
    } pb_reader_status;

    class PBReader
    {
    public:
        PBReader()
        {
            memset(&(this->status), 0, sizeof(pb_reader_status));
        }

        PBReader(pb_reader_status status)
        {
            this->status = status;
        }
        ~PBReader() {}

        /// @brief reads the next protobuf token from the input source.
        /// @return true if the token was read successfully; otherwise, false.
        bool Read();

        /// @brief Skips the children of the current protobuf token.
        void Skip();

        /// @brief Save the current status of the parser.
        void Save();

        /// @brief Restore the status previously saved.
        void Restore();

        pb_token_type_t GetTokenType()
        {
            return this->status.tokenType;
        }

    private:
        pb_reader_status status;
    };

    typedef PBReader *PBReaderPtr;
}

#endif