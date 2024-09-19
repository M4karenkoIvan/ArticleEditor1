#pragma once

#include "Document.h"

class Article final : public Document {
public:
  Type getType() const override {
    return Type::Article;
  }

    std::string_view getTitle() const {
    return title;
  }

  void setTitle(std::string_view newTitle) {
    title = newTitle;
  }

  std::string_view getText() const {
    return text;
  }

  void setText(std::string_view newText) {
    text = newText;
  }

private:
  std::string title;
  std::string text;
};
