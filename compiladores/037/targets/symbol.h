#ifndef UDF_SYMBOL_H
#define UDF_SYMBOL_H

#include <string>
#include <vector>
#include <memory>
#include <cdk/types/basic_type.h>

namespace udf {

  class symbol {
    std::shared_ptr<cdk::basic_type> _type;
    std::string _name;
    std::vector<std::shared_ptr<cdk::basic_type>> _args;
    int _offset;

  public:
    symbol(std::shared_ptr<cdk::basic_type> type, const std::string &name, int offset) :
        _type(type), _name(name), _offset(offset) {
    }

    symbol(std::shared_ptr<cdk::basic_type> type, const std::string &name, const std::vector<std::shared_ptr<cdk::basic_type>> &args) :
        _type(type), _name(name), _args(args) {
    }

    virtual ~symbol() {
      // EMPTY
    }

    std::shared_ptr<cdk::basic_type> type() const {
      return _type;
    }
    bool is_typed(cdk::typename_type name) const {
      return _type->name() == name;
    }
    const std::string &name() const {
      return _name;
    }
    const std::vector<std::shared_ptr<cdk::basic_type>> &args() const {
      return _args;
    }
    int offset() const {
      return _offset;
    }
    void offset(int offset) {
      _offset = offset;
    }
  };

} // udf

#endif
