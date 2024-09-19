#pragma once

#include "Article.h"
#include "Employee.h"

#include <iostream>
#include <vector>

class Journalist final : public Employee {
public:
  Journalist() : Employee("DefaultFirstName", "DefaultLastName", "DefaultDepartment") {}

  Journalist(std::string_view firstName, std::string_view lastName, std::string_view department)
      : Employee(firstName, lastName, department) {}

  virtual ~Journalist() = default;

  Position getPosition() const override {
    return Position::Journalist;
  }

  void createArticle() {
    std::string title;
    std::string text;
    std::cout << "Input title of the article: ";
    std::cin >> title;
    std::cout << "Input the artcile:\n";
    std::cin >> text;
    Article article;
    article.setTitle(title);
    article.setText(text);
    articles.push_back(article);
  }

  void viewArticles() {
    std::cout << "Current artcles: " << articles.size() << '\n';
    for (const auto& article : articles) {
      std::cout << article.getTitle() << '\n';
    }
  }

private:
  std::vector<Article> articles;
};
