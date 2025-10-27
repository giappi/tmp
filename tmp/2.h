#ifndef OMS_OMSREQUEST_H
#define OMS_OMSREQUEST_H

#include <cstdint>
#include <memory>
#include <string>
#include <vector>

namespace omsp {

// === Header có ý nghĩa ===
struct OmsHeader {
    static constexpr uint32_t MAGIC = 0x4F4D5300; // "OMS\0"
    uint32_t magic;
    uint32_t version;
    uint32_t body_length;
    uint64_t message_id;
};

// === Body trừu tượng ===
class OmsRequestBody {
public:

    virtual std::vector<uint8_t> serialize() const = 0;
    virtual std::pair<const uint8_t*, size_t> getRawView() const = 0;
    virtual ~OmsRequestBody() = default;
};

// === Request cơ sở ===
class OmsRequest {
public:
    virtual ~OmsRequest() = default;

    virtual std::string getAddress() const = 0;
    virtual const OmsHeader& getHeader() const = 0;
    virtual const OmsRequestBody& getBody() const = 0;
    virtual OmsMethod getMethod() const = 0;

    // Factory method trả về con trỏ thông minh
    static std::unique_ptr<OmsRequest> parse(const uint8_t* data, size_t length);

protected:
    OmsRequest(OmsHeader header, std::unique_ptr<OmsRequestBody> body)
        : _header(std::move(header)), _body(std::move(body)) {}

private:
    OmsHeader _header;
    std::unique_ptr<OmsRequestBody> _body;
};

// === Request cụ thể: GET ===
class OmsGetRequestBody : public OmsRequestBody {
    // ... dữ liệu cụ thể cho GET
public:
    std::vector<uint8_t> serialize() const override;
    std::pair<const uint8_t*, size_t> getRawView() const override;
};

class OmsGetRequest : public OmsRequest {
public:
    OmsGetRequest(OmsHeader header, std::unique_ptr<OmsGetRequestBody> body)
        : OmsRequest(std::move(header), std::move(body)) {}

    const OmsGetRequestBody& getBody() const override {
        return static_cast<const OmsGetRequestBody&>(
            OmsRequest::getBody()
        );
    }

    OmsMethod getMethod() const override { return OmsMethod::D_GET; }
    std::string getAddress() const override { /* ... */ }
};

} // namespace oms

#endif
