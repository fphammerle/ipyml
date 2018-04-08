#ifndef _IPYML_LIST_H
#define _IPYML_LIST_H

#include "yaml.h"

#include <list>    // std::list
#include <ostream> // std::ostream
#include <string>  // std::string

template <class T> class list : public std::list<T>, YamlObject {
public:
  void write_yaml(std::ostream &stream,
                  yaml_indent_level_t indent_level = 0) const {
    if (this->empty()) {
      stream << "[]";
    } else {
      if (indent_level > 2) {
        indent_level -= 2;
      }
      const std::string indent(indent_level, ' ');
      stream << "\n";
      for (auto it = this->begin(); it != this->end(); it++) {
        stream << indent << "- ";
        it->write_yaml(stream, indent_level + 2);
      }
    }
  }
};

#endif
