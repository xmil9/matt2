//
// Jun-2021, Michael Lindner
// MIT license
//
#include "placement_tests.h"
#include "placement.h"
#include "test_util.h"
#include <stdexcept>

using namespace matt2;


namespace
{
///////////////////

void testPlacementNotationCtor()
{
   {
      const std::string caseLabel = "Placement notation ctor for valid placements";

      VERIFY(Placement("Kbd8") == Placement(Kb, d8), caseLabel);
      VERIFY(Placement("Nwh2") == Placement(Nw, h2), caseLabel);
      VERIFY(Placement("bb4") == Placement(Pb, b4), caseLabel);
      VERIFY(Placement("wa7") == Placement(Pw, a7), caseLabel);
   }
   {
      const std::string caseLabel = "Placement notation ctor for invalid piece";

      try
      {
         auto p = Placement("Fbd8");
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
      const std::string caseLabel = "Placement notation ctor for invalid square";

      try
      {
         auto p = Placement("Rb8");
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
      const std::string caseLabel = "Placement notation ctor for empty string";

      try
      {
         auto p = Placement("");
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


void testPlacementEquality()
{
   {
      const std::string caseLabel = "Placement equality for equal instances";

      VERIFY(Placement(Kw, b4) == Placement(Kw, b4), caseLabel);
      VERIFY(Placement(Rb, a8) == Placement(Rb, a8), caseLabel);
      VERIFY(Placement(Pb, f6) == Placement(Pb, f6), caseLabel);
   }
   {
      const std::string caseLabel = "Placement equality for different pieces";

      VERIFY(!(Placement(Kw, b4) == Placement(Qw, b4)), caseLabel);
      VERIFY(!(Placement(Rb, a8) == Placement(Rw, a8)), caseLabel);
      VERIFY(!(Placement(Pb, f6) == Placement(Bb, f6)), caseLabel);
   }
   {
      const std::string caseLabel = "Placement equality for different squares";

      VERIFY(!(Placement(Kw, b4) == Placement(Kw, b5)), caseLabel);
      VERIFY(!(Placement(Rb, a8) == Placement(Rb, b8)), caseLabel);
      VERIFY(!(Placement(Pb, f6) == Placement(Pb, h1)), caseLabel);
   }
}


void testPlacementInequality()
{
   {
      const std::string caseLabel = "Placement inequality for different pieces";

      VERIFY(Placement(Kw, b4) != Placement(Qw, b4), caseLabel);
      VERIFY(Placement(Rb, a8) != Placement(Rw, a8), caseLabel);
      VERIFY(Placement(Pb, f6) != Placement(Bb, f6), caseLabel);
   }
   {
      const std::string caseLabel = "Placement inequality for different squares";

      VERIFY(Placement(Kw, b4) != Placement(Kw, b5), caseLabel);
      VERIFY(Placement(Rb, a8) != Placement(Rb, b8), caseLabel);
      VERIFY(Placement(Pb, f6) != Placement(Pb, h1), caseLabel);
   }
   {
      const std::string caseLabel = "Placement inequality for equal instances";

      VERIFY(!(Placement(Kw, b4) != Placement(Kw, b4)), caseLabel);
      VERIFY(!(Placement(Rb, a8) != Placement(Rb, a8)), caseLabel);
      VERIFY(!(Placement(Pb, f6) != Placement(Pb, f6)), caseLabel);
   }
}

} // namespace


///////////////////

void testPlacement()
{
   testPlacementNotationCtor();
   testPlacementEquality();
   testPlacementInequality();
}
