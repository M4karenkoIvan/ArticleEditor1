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

  void setPhoto(std::string_view newPhoto) {
    photo = newPhoto;
  }

  std::string_view getCaption() const {
    return caption;
  }

  void setCaption(std::string_view newCaption) {
    caption = newCaption;
  }

private:
  std::string photo;
  std::string caption;
};
