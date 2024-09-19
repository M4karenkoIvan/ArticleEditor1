#pragma once

#include "Document.h"

class PhotoReport final : public Document {
public:
  Type getType() const override {
    return Type::PhotoReport;
  }

  std::string_view getPhoto() const {
    return photo;
  }

  void setPhoto() {
    
  }

private:
  std::string photo;
};
