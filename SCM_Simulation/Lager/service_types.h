/**
 * Autogenerated by Thrift Compiler (0.15.0)
 *
 * DO NOT EDIT UNLESS YOU ARE SURE THAT YOU KNOW WHAT YOU ARE DOING
 *  @generated
 */
#ifndef service_TYPES_H
#define service_TYPES_H

#include <iosfwd>

#include <thrift/Thrift.h>
#include <thrift/TApplicationException.h>
#include <thrift/TBase.h>
#include <thrift/protocol/TProtocol.h>
#include <thrift/transport/TTransport.h>

#include <functional>
#include <memory>


namespace Service {

class Product;

typedef struct _Product__isset {
  _Product__isset() : name(false), id(false), amount(false) {}
  bool name :1;
  bool id :1;
  bool amount :1;
} _Product__isset;

class Product : public virtual ::apache::thrift::TBase {
 public:

  Product(const Product&);
  Product& operator=(const Product&);
  Product() noexcept
          : name(),
            id(0),
            amount(0) {
  }

  virtual ~Product() noexcept;
  std::string name;
  int32_t id;
  int32_t amount;

  _Product__isset __isset;

  void __set_name(const std::string& val);

  void __set_id(const int32_t val);

  void __set_amount(const int32_t val);

  bool operator == (const Product & rhs) const
  {
    if (!(name == rhs.name))
      return false;
    if (!(id == rhs.id))
      return false;
    if (!(amount == rhs.amount))
      return false;
    return true;
  }
  bool operator != (const Product &rhs) const {
    return !(*this == rhs);
  }

  bool operator < (const Product & ) const;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot) override;
  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const override;

  virtual void printTo(std::ostream& out) const;
};

void swap(Product &a, Product &b);

std::ostream& operator<<(std::ostream& out, const Product& obj);

} // namespace

#endif
