#ifndef OMS_OMSREQUEST_H
#define OMS_OMSREQUEST_H
#include <cstdint>
#include <memory>
#include <string>
#include <vector>

namespace oms::serializable
{
    struct AbcHeader {};
    struct NetHeader {};
    struct DbaHeader {};
    struct OmsHeader {};
}

namespace oms
{
    enum class OmsMethod
    {
        GET,
        PUT
    };

    class OmsRequestBody
    {
    public:
        virtual ~OmsRequestBody() = default;

        virtual std::pair<const uint8_t*, size_t>
                                            getRawView() const;
    private:
        uint8_t*  _rawData;
    };
    using OmsRequestBodyPtr = std::shared_ptr<OmsRequestBody>;


    class OmsRequest
    {
        using OmsRequestPtr = std::shared_ptr<OmsRequest>;
        using OmsHeader = serializable::OmsHeader;
    public:
        virtual ~OmsRequest() = default;

        virtual std::string                 getAddress();
        virtual uint16_t                    getPort();
        virtual const OmsHeader&            getHeader();
        virtual std::vector<uint8_t>        getRawBody() const = 0;
        virtual OmsMethod                   getMethod() const = 0;
        virtual std::vector<uint8_t>        serialize() const = 0;
        virtual std::pair<const uint8_t*, size_t>
                                            getRawView() const;

        static OmsRequestPtr parse(uint8_t* data, size_t length);

    };
}


namespace oms
{
    class OmsGetRequestBody;
    using OmsGetRequestBodyPtr = std::shared_ptr<OmsGetRequestBody>;

    class OmsGetRequest: public OmsRequest
    {
    public:
        std::vector<uint8_t>        getRawBody() const override;
        inline OmsMethod            getMethod() const override { return OmsMethod::GET; };
        std::vector<uint8_t>        serialize() const override;
        OmsGetRequestBodyPtr        getBody() const;

    };

}



#endif //OMS_OMSREQUEST_H
