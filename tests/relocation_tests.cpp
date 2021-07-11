//
// Jun-2021, Michael Lindner
// MIT license
//
#include "relocation_tests.h"
#include "relocation.h"
#include "test_util.h"
#include <stdexcept>

using namespace matt2;


namespace
{
///////////////////

void testRelocationNotationCtor()
{
   {
      const std::string caseLabel = "Relocation notation ctor for valid relocations";

      VERIFY(Relocation("Kbd8c7") == Relocation(Kb, d8, c7), caseLabel);
      VERIFY(Relocation("Nwh2g4") == Relocation(Nw, h2, g4), caseLabel);
      VERIFY(Relocation("bb4b3") == Relocation(Pb, b4, b3), caseLabel);
      VERIFY(Relocation("wa5b6") == Relocation(Pw, a5, b6), caseLabel);
   }
   {
      const std::string caseLabel = "Relocation notation ctor for invalid piece";

      try
      {
         auto rl = Relocation("Fbd8d6");
         FAIL("Exception expected.", caseLabel);
      }
      catch (std::runtime_error&)
      {
         // Expected
      }
      catch (...)
      {
         FAIL("Other exception type expected.", caseLabel);
      }
   }
   {
      const std::string caseLabel = "Relocation notation ctor for invalid source square";

      try
      {
         auto rl = Relocation("Rb8f4");
         FAIL("Exception expected.", caseLabel);
      }
      catch (std::runtime_error&)
      {
         // Expected
      }
      catch (...)
      {
         FAIL("Other exception type expected.", caseLabel);
      }
   }
   {
      const std::string caseLabel = "Relocation notation ctor for invalid target square";

      try
      {
         auto rl = Relocation("Rbf8f");
         FAIL("Exception expected.", caseLabel);
      }
      catch (std::runtime_error&)
      {
         // Expected
      }
      catch (...)
      {
         FAIL("Other exception type expected.", caseLabel);
      }
   }
   {
      const std::string caseLabel = "Relocation notation ctor for empty string";

      try
      {
         auto rl = Relocation("");
         FAIL("Exception expected.", caseLabel);
      }
      catch (std::runtime_error&)
      {
         // Expected
      }
      catch (...)
      {
         FAIL("Other exception type expected.", caseLabel);
      }
   }
}


void testRelocationPiece()
{
   {
      const std::string caseLabel = "Relocation::piece()";

      VERIFY(Relocation(Rb, a8, f8).piece() == Rb, caseLabel);
      VERIFY(Relocation(Kw, b4, c3).piece() == Kw, caseLabel);
   }
}


void testRelocationFrom()
{
   {
      const std::string caseLabel = "Relocation::from()";

      VERIFY(Relocation(Rb, a8, f8).from() == a8, caseLabel);
      VERIFY(Relocation(Kw, b4, c3).from() == b4, caseLabel);
   }
}


void testRelocationTo()
{
   {
      const std::string caseLabel = "Relocation::to()";

      VERIFY(Relocation(Rb, a8, f8).to() == f8, caseLabel);
      VERIFY(Relocation(Kw, b4, c3).to() == c3, caseLabel);
   }
}


void testRelocationEquality()
{
   {
      const std::string caseLabel = "Relocation equality for different pieces";

      VERIFY(!(Relocation(Kw, b4, c3) == Relocation(Qw, b4, c3)), caseLabel);
      VERIFY(!(Relocation(Rb, a8, f8) == Relocation(Rw, a8, f8)), caseLabel);
      VERIFY(!(Relocation(Pb, f6, f5) == Relocation(Bb, f6, f5)), caseLabel);
   }
   {
      const std::string caseLabel = "Relocation equality for different source squares";

      VERIFY(!(Relocation(Kw, b4, c3) == Relocation(Kw, b5, c3)), caseLabel);
      VERIFY(!(Relocation(Rb, a8, f8) == Relocation(Rb, b8, f8)), caseLabel);
      VERIFY(!(Relocation(Pb, f6, f5) == Relocation(Pb, h1, f5)), caseLabel);
   }
   {
      const std::string caseLabel = "Relocation equality for different target squares";

      VERIFY(!(Relocation(Kw, b4, c3) == Relocation(Kw, b4, c4)), caseLabel);
      VERIFY(!(Relocation(Rb, a8, f8) == Relocation(Rb, a8, f7)), caseLabel);
      VERIFY(!(Relocation(Pb, f6, f5) == Relocation(Pb, f6, f6)), caseLabel);
   }
   {
      const std::string caseLabel = "Relocation equality for equal instances";

      VERIFY(Relocation(Kw, b4, c3) == Relocation(Kw, b4, c3), caseLabel);
      VERIFY(Relocation(Rb, a8, f8) == Relocation(Rb, a8, f8), caseLabel);
      VERIFY(Relocation(Pb, f6, f5) == Relocation(Pb, f6, f5), caseLabel);
   }
}


void testRelocationInequality()
{
   {
      const std::string caseLabel = "Relocation inequality for different pieces";

      VERIFY(Relocation(Kw, b4, c3) != Relocation(Qw, b4, c3), caseLabel);
      VERIFY(Relocation(Rb, a8, f8) != Relocation(Rw, a8, f8), caseLabel);
      VERIFY(Relocation(Pb, f6, f5) != Relocation(Bb, f6, f5), caseLabel);
   }
   {
      const std::string caseLabel = "Relocation inequality for different source squares";

      VERIFY(Relocation(Kw, b4, c3) != Relocation(Kw, b5, c3), caseLabel);
      VERIFY(Relocation(Rb, a8, f8) != Relocation(Rb, b8, f8), caseLabel);
      VERIFY(Relocation(Pb, f6, f5) != Relocation(Pb, h1, f5), caseLabel);
   }
   {
      const std::string caseLabel = "Relocation inequality for different target squares";

      VERIFY(Relocation(Kw, b4, c3) != Relocation(Kw, b4, c4), caseLabel);
      VERIFY(Relocation(Rb, a8, f8) != Relocation(Rb, a8, f7), caseLabel);
      VERIFY(Relocation(Pb, f6, f5) != Relocation(Pb, f6, f6), caseLabel);
   }
   {
      const std::string caseLabel = "Relocation inequality for equal instances";

      VERIFY(!(Relocation(Kw, b4, c3) != Relocation(Kw, b4, c3)), caseLabel);
      VERIFY(!(Relocation(Rb, a8, f8) != Relocation(Rb, a8, f8)), caseLabel);
      VERIFY(!(Relocation(Pb, f6, f5) != Relocation(Pb, f6, f5)), caseLabel);
   }
}


void testRelocationReverse()
{
   {
      const std::string caseLabel = "Relocation::reverse";

      VERIFY(Relocation(Kw, b4, c3).reverse() == Relocation(Kw, c3, b4), caseLabel);
      VERIFY(Relocation(Rb, a8, f8).reverse() == Relocation(Rb, f8, a8), caseLabel);
   }
}

} // namespace


///////////////////

void testRelocation()
{
   testRelocationNotationCtor();
   testRelocationPiece();
   testRelocationFrom();
   testRelocationTo();
   testRelocationEquality();
   testRelocationInequality();
   testRelocationReverse();
}
