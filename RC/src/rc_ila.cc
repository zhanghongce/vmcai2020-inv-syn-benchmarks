#include <rc_ila.h>

using namespace ilang;

Ila BuildModel() {
  auto m = Ila("counter");
  auto en = m.NewBvInput("en", 1);
  auto v  = m.NewBvState("v" , 4);
  auto INC = m.NewInstr("INC");
  {
    INC.SetDecode(en == 1);
    INC.SetUpdate(v, v + 1);
  }
  return m;
}

