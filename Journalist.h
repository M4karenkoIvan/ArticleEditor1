#pragma once

#include "Employee.h"
#include <vector>
#include "Article.h"
#include <iostream>

class Journalist final : public Employee {
public:
  virtual ~Journalist() = default;

  std::string_view getPosition() const override {
    return "journalist";
  }

  void createArticle() {
    std::cout <<" input tittle of the article" << std::endl;
    Article::setTitle;
    std::cout << "input the artcile:" << std::endl;
    Article::setText;
  };

private:
  std::vector<Article> articles;
};
