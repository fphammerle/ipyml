#ifndef _IPYML_YAML_H
#define _IPYML_YAML_H

#include <ostream> // std::ostream

typedef unsigned char yaml_indent_level_t;

class YamlObject {
  virtual void write_yaml(std::ostream &stream,
                          yaml_indent_level_t indent_level = 0) const = 0;
};

#endif
