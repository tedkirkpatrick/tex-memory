#ifndef BOX_CLASSES_HPP
#define BOX_CLASSES_HPP

class Box {
public:
  Box();
private:
  Box* link {nullptr};
};

class HList : public Box {
public:
  HList();
private:
  Box* contents {nullptr};
};

#endif
