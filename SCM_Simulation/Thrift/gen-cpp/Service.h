/**
 * Autogenerated by Thrift Compiler (0.15.0)
 *
 * DO NOT EDIT UNLESS YOU ARE SURE THAT YOU KNOW WHAT YOU ARE DOING
 *  @generated
 */
#ifndef Service_H
#define Service_H

#include <thrift/TDispatchProcessor.h>
#include <thrift/async/TConcurrentClientSyncInfo.h>
#include <memory>
#include "service_types.h"

namespace Service {

#ifdef _MSC_VER
  #pragma warning( push )
  #pragma warning (disable : 4250 ) //inheriting methods via dominance 
#endif

class ServiceIf {
 public:
  virtual ~ServiceIf() {}
  virtual void seeProducts(std::vector<Product> & _return) = 0;
  virtual void orderProduct(std::string& _return, const std::string& product, const int32_t amount) = 0;
};

class ServiceIfFactory {
 public:
  typedef ServiceIf Handler;

  virtual ~ServiceIfFactory() {}

  virtual ServiceIf* getHandler(const ::apache::thrift::TConnectionInfo& connInfo) = 0;
  virtual void releaseHandler(ServiceIf* /* handler */) = 0;
  };

class ServiceIfSingletonFactory : virtual public ServiceIfFactory {
 public:
  ServiceIfSingletonFactory(const ::std::shared_ptr<ServiceIf>& iface) : iface_(iface) {}
  virtual ~ServiceIfSingletonFactory() {}

  virtual ServiceIf* getHandler(const ::apache::thrift::TConnectionInfo&) override {
    return iface_.get();
  }
  virtual void releaseHandler(ServiceIf* /* handler */) override {}

 protected:
  ::std::shared_ptr<ServiceIf> iface_;
};

class ServiceNull : virtual public ServiceIf {
 public:
  virtual ~ServiceNull() {}
  void seeProducts(std::vector<Product> & /* _return */) override {
    return;
  }
  void orderProduct(std::string& /* _return */, const std::string& /* product */, const int32_t /* amount */) override {
    return;
  }
};


class Service_seeProducts_args {
 public:

  Service_seeProducts_args(const Service_seeProducts_args&) noexcept;
  Service_seeProducts_args& operator=(const Service_seeProducts_args&) noexcept;
  Service_seeProducts_args() noexcept {
  }

  virtual ~Service_seeProducts_args() noexcept;

  bool operator == (const Service_seeProducts_args & /* rhs */) const
  {
    return true;
  }
  bool operator != (const Service_seeProducts_args &rhs) const {
    return !(*this == rhs);
  }

  bool operator < (const Service_seeProducts_args & ) const;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);
  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

};


class Service_seeProducts_pargs {
 public:


  virtual ~Service_seeProducts_pargs() noexcept;

  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

};

typedef struct _Service_seeProducts_result__isset {
  _Service_seeProducts_result__isset() : success(false) {}
  bool success :1;
} _Service_seeProducts_result__isset;

class Service_seeProducts_result {
 public:

  Service_seeProducts_result(const Service_seeProducts_result&);
  Service_seeProducts_result& operator=(const Service_seeProducts_result&);
  Service_seeProducts_result() noexcept {
  }

  virtual ~Service_seeProducts_result() noexcept;
  std::vector<Product>  success;

  _Service_seeProducts_result__isset __isset;

  void __set_success(const std::vector<Product> & val);

  bool operator == (const Service_seeProducts_result & rhs) const
  {
    if (!(success == rhs.success))
      return false;
    return true;
  }
  bool operator != (const Service_seeProducts_result &rhs) const {
    return !(*this == rhs);
  }

  bool operator < (const Service_seeProducts_result & ) const;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);
  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

};

typedef struct _Service_seeProducts_presult__isset {
  _Service_seeProducts_presult__isset() : success(false) {}
  bool success :1;
} _Service_seeProducts_presult__isset;

class Service_seeProducts_presult {
 public:


  virtual ~Service_seeProducts_presult() noexcept;
  std::vector<Product> * success;

  _Service_seeProducts_presult__isset __isset;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);

};

typedef struct _Service_orderProduct_args__isset {
  _Service_orderProduct_args__isset() : product(false), amount(false) {}
  bool product :1;
  bool amount :1;
} _Service_orderProduct_args__isset;

class Service_orderProduct_args {
 public:

  Service_orderProduct_args(const Service_orderProduct_args&);
  Service_orderProduct_args& operator=(const Service_orderProduct_args&);
  Service_orderProduct_args() noexcept
                            : product(),
                              amount(0) {
  }

  virtual ~Service_orderProduct_args() noexcept;
  std::string product;
  int32_t amount;

  _Service_orderProduct_args__isset __isset;

  void __set_product(const std::string& val);

  void __set_amount(const int32_t val);

  bool operator == (const Service_orderProduct_args & rhs) const
  {
    if (!(product == rhs.product))
      return false;
    if (!(amount == rhs.amount))
      return false;
    return true;
  }
  bool operator != (const Service_orderProduct_args &rhs) const {
    return !(*this == rhs);
  }

  bool operator < (const Service_orderProduct_args & ) const;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);
  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

};


class Service_orderProduct_pargs {
 public:


  virtual ~Service_orderProduct_pargs() noexcept;
  const std::string* product;
  const int32_t* amount;

  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

};

typedef struct _Service_orderProduct_result__isset {
  _Service_orderProduct_result__isset() : success(false) {}
  bool success :1;
} _Service_orderProduct_result__isset;

class Service_orderProduct_result {
 public:

  Service_orderProduct_result(const Service_orderProduct_result&);
  Service_orderProduct_result& operator=(const Service_orderProduct_result&);
  Service_orderProduct_result() noexcept
                              : success() {
  }

  virtual ~Service_orderProduct_result() noexcept;
  std::string success;

  _Service_orderProduct_result__isset __isset;

  void __set_success(const std::string& val);

  bool operator == (const Service_orderProduct_result & rhs) const
  {
    if (!(success == rhs.success))
      return false;
    return true;
  }
  bool operator != (const Service_orderProduct_result &rhs) const {
    return !(*this == rhs);
  }

  bool operator < (const Service_orderProduct_result & ) const;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);
  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

};

typedef struct _Service_orderProduct_presult__isset {
  _Service_orderProduct_presult__isset() : success(false) {}
  bool success :1;
} _Service_orderProduct_presult__isset;

class Service_orderProduct_presult {
 public:


  virtual ~Service_orderProduct_presult() noexcept;
  std::string* success;

  _Service_orderProduct_presult__isset __isset;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);

};

class ServiceClient : virtual public ServiceIf {
 public:
  ServiceClient(std::shared_ptr< ::apache::thrift::protocol::TProtocol> prot) {
    setProtocol(prot);
  }
  ServiceClient(std::shared_ptr< ::apache::thrift::protocol::TProtocol> iprot, std::shared_ptr< ::apache::thrift::protocol::TProtocol> oprot) {
    setProtocol(iprot,oprot);
  }
 private:
  void setProtocol(std::shared_ptr< ::apache::thrift::protocol::TProtocol> prot) {
  setProtocol(prot,prot);
  }
  void setProtocol(std::shared_ptr< ::apache::thrift::protocol::TProtocol> iprot, std::shared_ptr< ::apache::thrift::protocol::TProtocol> oprot) {
    piprot_=iprot;
    poprot_=oprot;
    iprot_ = iprot.get();
    oprot_ = oprot.get();
  }
 public:
  std::shared_ptr< ::apache::thrift::protocol::TProtocol> getInputProtocol() {
    return piprot_;
  }
  std::shared_ptr< ::apache::thrift::protocol::TProtocol> getOutputProtocol() {
    return poprot_;
  }
  void seeProducts(std::vector<Product> & _return) override;
  void send_seeProducts();
  void recv_seeProducts(std::vector<Product> & _return);
  void orderProduct(std::string& _return, const std::string& product, const int32_t amount) override;
  void send_orderProduct(const std::string& product, const int32_t amount);
  void recv_orderProduct(std::string& _return);
 protected:
  std::shared_ptr< ::apache::thrift::protocol::TProtocol> piprot_;
  std::shared_ptr< ::apache::thrift::protocol::TProtocol> poprot_;
  ::apache::thrift::protocol::TProtocol* iprot_;
  ::apache::thrift::protocol::TProtocol* oprot_;
};

class ServiceProcessor : public ::apache::thrift::TDispatchProcessor {
 protected:
  ::std::shared_ptr<ServiceIf> iface_;
  virtual bool dispatchCall(::apache::thrift::protocol::TProtocol* iprot, ::apache::thrift::protocol::TProtocol* oprot, const std::string& fname, int32_t seqid, void* callContext) override;
 private:
  typedef  void (ServiceProcessor::*ProcessFunction)(int32_t, ::apache::thrift::protocol::TProtocol*, ::apache::thrift::protocol::TProtocol*, void*);
  typedef std::map<std::string, ProcessFunction> ProcessMap;
  ProcessMap processMap_;
  void process_seeProducts(int32_t seqid, ::apache::thrift::protocol::TProtocol* iprot, ::apache::thrift::protocol::TProtocol* oprot, void* callContext);
  void process_orderProduct(int32_t seqid, ::apache::thrift::protocol::TProtocol* iprot, ::apache::thrift::protocol::TProtocol* oprot, void* callContext);
 public:
  ServiceProcessor(::std::shared_ptr<ServiceIf> iface) :
    iface_(iface) {
    processMap_["seeProducts"] = &ServiceProcessor::process_seeProducts;
    processMap_["orderProduct"] = &ServiceProcessor::process_orderProduct;
  }

  virtual ~ServiceProcessor() {}
};

class ServiceProcessorFactory : public ::apache::thrift::TProcessorFactory {
 public:
  ServiceProcessorFactory(const ::std::shared_ptr< ServiceIfFactory >& handlerFactory) noexcept :
      handlerFactory_(handlerFactory) {}

  ::std::shared_ptr< ::apache::thrift::TProcessor > getProcessor(const ::apache::thrift::TConnectionInfo& connInfo) override;

 protected:
  ::std::shared_ptr< ServiceIfFactory > handlerFactory_;
};

class ServiceMultiface : virtual public ServiceIf {
 public:
  ServiceMultiface(std::vector<std::shared_ptr<ServiceIf> >& ifaces) : ifaces_(ifaces) {
  }
  virtual ~ServiceMultiface() {}
 protected:
  std::vector<std::shared_ptr<ServiceIf> > ifaces_;
  ServiceMultiface() {}
  void add(::std::shared_ptr<ServiceIf> iface) {
    ifaces_.push_back(iface);
  }
 public:
  void seeProducts(std::vector<Product> & _return) override {
    size_t sz = ifaces_.size();
    size_t i = 0;
    for (; i < (sz - 1); ++i) {
      ifaces_[i]->seeProducts(_return);
    }
    ifaces_[i]->seeProducts(_return);
    return;
  }

  void orderProduct(std::string& _return, const std::string& product, const int32_t amount) override {
    size_t sz = ifaces_.size();
    size_t i = 0;
    for (; i < (sz - 1); ++i) {
      ifaces_[i]->orderProduct(_return, product, amount);
    }
    ifaces_[i]->orderProduct(_return, product, amount);
    return;
  }

};

// The 'concurrent' client is a thread safe client that correctly handles
// out of order responses.  It is slower than the regular client, so should
// only be used when you need to share a connection among multiple threads
class ServiceConcurrentClient : virtual public ServiceIf {
 public:
  ServiceConcurrentClient(std::shared_ptr< ::apache::thrift::protocol::TProtocol> prot, std::shared_ptr<::apache::thrift::async::TConcurrentClientSyncInfo> sync) : sync_(sync)
{
    setProtocol(prot);
  }
  ServiceConcurrentClient(std::shared_ptr< ::apache::thrift::protocol::TProtocol> iprot, std::shared_ptr< ::apache::thrift::protocol::TProtocol> oprot, std::shared_ptr<::apache::thrift::async::TConcurrentClientSyncInfo> sync) : sync_(sync)
{
    setProtocol(iprot,oprot);
  }
 private:
  void setProtocol(std::shared_ptr< ::apache::thrift::protocol::TProtocol> prot) {
  setProtocol(prot,prot);
  }
  void setProtocol(std::shared_ptr< ::apache::thrift::protocol::TProtocol> iprot, std::shared_ptr< ::apache::thrift::protocol::TProtocol> oprot) {
    piprot_=iprot;
    poprot_=oprot;
    iprot_ = iprot.get();
    oprot_ = oprot.get();
  }
 public:
  std::shared_ptr< ::apache::thrift::protocol::TProtocol> getInputProtocol() {
    return piprot_;
  }
  std::shared_ptr< ::apache::thrift::protocol::TProtocol> getOutputProtocol() {
    return poprot_;
  }
  void seeProducts(std::vector<Product> & _return) override;
  int32_t send_seeProducts();
  void recv_seeProducts(std::vector<Product> & _return, const int32_t seqid);
  void orderProduct(std::string& _return, const std::string& product, const int32_t amount) override;
  int32_t send_orderProduct(const std::string& product, const int32_t amount);
  void recv_orderProduct(std::string& _return, const int32_t seqid);
 protected:
  std::shared_ptr< ::apache::thrift::protocol::TProtocol> piprot_;
  std::shared_ptr< ::apache::thrift::protocol::TProtocol> poprot_;
  ::apache::thrift::protocol::TProtocol* iprot_;
  ::apache::thrift::protocol::TProtocol* oprot_;
  std::shared_ptr<::apache::thrift::async::TConcurrentClientSyncInfo> sync_;
};

#ifdef _MSC_VER
  #pragma warning( pop )
#endif

} // namespace

#endif
