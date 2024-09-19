#pragma once

#include "Document.h"

class MagazineLayout final : public Document {
public:
  Type getType() const override {
    return Type::MagazineLayout;
  }

  std::string_view getLayout() const {
    return layout;
  }

  void setLayout(std::string_view newLayout) {
    layout = newLayout;
  }

  std::string_view getTypeLayout() const {
    return typeLayout;
  }

  void setTypeLayout(std::string_view newtypeLayout) {
    typeLayout = newtypeLayout;
  }

private:
  std::string layout;
  std::string typeLayout;
};