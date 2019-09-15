#include "catch.hpp"
#include "..\include\netbpm.hpp"


TEST_CASE("Default construction", "[netbpm_construct]")
{
	netbpm data;

	REQUIRE(data.width() == 0);
	REQUIRE(data.height() == 0);
	REQUIRE(data.channels() == 0);
	REQUIRE(data.bpp() == 0);
	REQUIRE(data.range() == 0);
}

// Cionstruct from raw bits...


// construct from file... binary

// construct from file... ascii


// construct from string... ascii



