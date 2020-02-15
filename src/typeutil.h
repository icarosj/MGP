
template <class T>
std::string getType();


template <>
std::string getType<int>()
{
  return std::string("int");
}
template <>
std::string getType<float>()
{
  return std::string("float");
}

