#include <tradelayer/dex.h>
#include <test/test_bitcoin.h>
#include <boost/test/unit_test.hpp>
#include <stdint.h>

using namespace mastercore;

BOOST_FIXTURE_TEST_SUITE(tradelayer_dex_functions_tests, BasicTestingSetup)


BOOST_AUTO_TEST_CASE(calculate_desiredltc)
{

    // 1 LTC = 1 ALL
    int64_t amountOffered = 400000000; // 4 ALL
    int64_t amountDesired = 400000000; // 4 LTCs
    int64_t amountAvailable = 300000000; // 3 ALL (left to offer)

    // checking ((amountDesiredOfBitcoin * amountAvailableOfTokens) / initialAmountOfTokens)
    BOOST_CHECK_EQUAL(300000000, mastercore::calculateDesiredLTC(amountOffered, amountDesired, amountAvailable));

    // 10 LTC = 1 ALL
    amountOffered = 500000000; // 5 ALL
    amountDesired = 5000000000; // 50 LTCs
    amountAvailable = 200000000; // 2 ALL (left to offer)
    BOOST_CHECK_EQUAL(2000000000, mastercore::calculateDesiredLTC(amountOffered, amountDesired, amountAvailable));

    // 1 LTC = 10 ALL
    amountOffered = 5000000000; // 50 ALL
    amountDesired = 500000000; // 5 LTCs
    amountAvailable = 2500000000; // 25 ALL (left to offer)
    BOOST_CHECK_EQUAL(250000000, mastercore::calculateDesiredLTC(amountOffered, amountDesired, amountAvailable));

    // 1 LTC = 1 ALL
    amountOffered = numeric_limits<int64_t>::max(); // 92,233,720,368.54775807 ALL
    amountDesired = numeric_limits<int64_t>::max(); // 92,233,720,368.54775807 LTC
    amountAvailable = 50000000000; // 500 ALL (left to offer)
    BOOST_CHECK_EQUAL(50000000000, mastercore::calculateDesiredLTC(amountOffered, amountDesired, amountAvailable));

    // Increasing amountOffered
    BOOST_CHECK_EQUAL(1000000000000000000, mastercore::calculateDesiredLTC(1, 1000000000, 1000000000));
    BOOST_CHECK_EQUAL(100000000000000000, mastercore::calculateDesiredLTC(10, 1000000000, 1000000000));
    BOOST_CHECK_EQUAL(10000000000000000, mastercore::calculateDesiredLTC(100, 1000000000, 1000000000));
    BOOST_CHECK_EQUAL(1000000000000000, mastercore::calculateDesiredLTC(1000, 1000000000, 1000000000));
    BOOST_CHECK_EQUAL(100000000000000, mastercore::calculateDesiredLTC(10000, 1000000000, 1000000000));
    BOOST_CHECK_EQUAL(10000000000000, mastercore::calculateDesiredLTC(100000, 1000000000, 1000000000));
    BOOST_CHECK_EQUAL(1000000000000, mastercore::calculateDesiredLTC(1000000, 1000000000, 1000000000));
    BOOST_CHECK_EQUAL(100000000000, mastercore::calculateDesiredLTC(10000000, 1000000000, 1000000000));
    BOOST_CHECK_EQUAL(10000000000, mastercore::calculateDesiredLTC(100000000, 1000000000, 1000000000));
    BOOST_CHECK_EQUAL(1000000000, mastercore::calculateDesiredLTC(1000000000, 1000000000, 1000000000));
    BOOST_CHECK_EQUAL(100000000, mastercore::calculateDesiredLTC(10000000000, 1000000000, 1000000000));
    BOOST_CHECK_EQUAL(10000000, mastercore::calculateDesiredLTC(100000000000, 1000000000, 1000000000));
    BOOST_CHECK_EQUAL(1000000, mastercore::calculateDesiredLTC(1000000000000, 1000000000, 1000000000));
    BOOST_CHECK_EQUAL(100000, mastercore::calculateDesiredLTC(10000000000000, 1000000000, 1000000000));
    BOOST_CHECK_EQUAL(10000, mastercore::calculateDesiredLTC(100000000000000, 1000000000, 1000000000));
    BOOST_CHECK_EQUAL(1000, mastercore::calculateDesiredLTC(1000000000000000, 1000000000, 1000000000));
    BOOST_CHECK_EQUAL(100, mastercore::calculateDesiredLTC(10000000000000000, 1000000000, 1000000000));
    BOOST_CHECK_EQUAL(10, mastercore::calculateDesiredLTC(100000000000000000, 1000000000, 1000000000));
    BOOST_CHECK_EQUAL(1, mastercore::calculateDesiredLTC(1000000000000000000, 1000000000, 1000000000));

    // Increasing amountDesired
    BOOST_CHECK_EQUAL(1, mastercore::calculateDesiredLTC(1000000000, 1, 1000000000));
    BOOST_CHECK_EQUAL(10, mastercore::calculateDesiredLTC(1000000000, 10, 1000000000));
    BOOST_CHECK_EQUAL(100, mastercore::calculateDesiredLTC(1000000000, 100, 1000000000));
    BOOST_CHECK_EQUAL(1000, mastercore::calculateDesiredLTC(1000000000, 1000, 1000000000));
    BOOST_CHECK_EQUAL(10000, mastercore::calculateDesiredLTC(1000000000, 10000, 1000000000));
    BOOST_CHECK_EQUAL(100000, mastercore::calculateDesiredLTC(1000000000, 100000, 1000000000));
    BOOST_CHECK_EQUAL(1000000, mastercore::calculateDesiredLTC(1000000000, 1000000, 1000000000));
    BOOST_CHECK_EQUAL(10000000, mastercore::calculateDesiredLTC(1000000000, 10000000, 1000000000));
    BOOST_CHECK_EQUAL(100000000, mastercore::calculateDesiredLTC(1000000000, 100000000, 1000000000));
    BOOST_CHECK_EQUAL(1000000000, mastercore::calculateDesiredLTC(1000000000, 1000000000, 1000000000));
    BOOST_CHECK_EQUAL(10000000000, mastercore::calculateDesiredLTC(1000000000, 10000000000, 1000000000));
    BOOST_CHECK_EQUAL(100000000000, mastercore::calculateDesiredLTC(1000000000, 100000000000, 1000000000));
    BOOST_CHECK_EQUAL(1000000000000, mastercore::calculateDesiredLTC(1000000000, 1000000000000, 1000000000));
    BOOST_CHECK_EQUAL(10000000000000, mastercore::calculateDesiredLTC(1000000000, 10000000000000, 1000000000));
    BOOST_CHECK_EQUAL(100000000000000, mastercore::calculateDesiredLTC(1000000000, 100000000000000, 1000000000));
    BOOST_CHECK_EQUAL(1000000000000000, mastercore::calculateDesiredLTC(1000000000, 1000000000000000, 1000000000));
    BOOST_CHECK_EQUAL(10000000000000000, mastercore::calculateDesiredLTC(1000000000, 10000000000000000, 1000000000));
    BOOST_CHECK_EQUAL(100000000000000000, mastercore::calculateDesiredLTC(1000000000, 100000000000000000, 1000000000));
    BOOST_CHECK_EQUAL(1000000000000000000, mastercore::calculateDesiredLTC(1000000000, 1000000000000000000, 1000000000));

    // Increasing amountAvailable
    BOOST_CHECK_EQUAL(1, mastercore::calculateDesiredLTC(1000000000, 1000000000, 1));
    BOOST_CHECK_EQUAL(10, mastercore::calculateDesiredLTC(1000000000, 1000000000, 10));
    BOOST_CHECK_EQUAL(100, mastercore::calculateDesiredLTC(1000000000, 1000000000, 100));
    BOOST_CHECK_EQUAL(1000, mastercore::calculateDesiredLTC(1000000000, 1000000000, 1000));
    BOOST_CHECK_EQUAL(10000, mastercore::calculateDesiredLTC(1000000000, 1000000000, 10000));
    BOOST_CHECK_EQUAL(100000, mastercore::calculateDesiredLTC(1000000000, 1000000000, 100000));
    BOOST_CHECK_EQUAL(1000000, mastercore::calculateDesiredLTC(1000000000, 1000000000, 1000000));
    BOOST_CHECK_EQUAL(10000000, mastercore::calculateDesiredLTC(1000000000, 1000000000, 10000000));
    BOOST_CHECK_EQUAL(100000000, mastercore::calculateDesiredLTC(1000000000, 1000000000, 100000000));
    BOOST_CHECK_EQUAL(1000000000, mastercore::calculateDesiredLTC(1000000000, 1000000000, 1000000000));
    BOOST_CHECK_EQUAL(10000000000, mastercore::calculateDesiredLTC(1000000000, 1000000000, 10000000000));
    BOOST_CHECK_EQUAL(100000000000, mastercore::calculateDesiredLTC(1000000000, 1000000000, 100000000000));
    BOOST_CHECK_EQUAL(1000000000000, mastercore::calculateDesiredLTC(1000000000, 1000000000, 1000000000000));
    BOOST_CHECK_EQUAL(10000000000000, mastercore::calculateDesiredLTC(1000000000, 1000000000, 10000000000000));
    BOOST_CHECK_EQUAL(100000000000000, mastercore::calculateDesiredLTC(1000000000, 1000000000, 100000000000000));
    BOOST_CHECK_EQUAL(1000000000000000, mastercore::calculateDesiredLTC(1000000000, 1000000000, 1000000000000000));
    BOOST_CHECK_EQUAL(10000000000000000, mastercore::calculateDesiredLTC(1000000000, 1000000000, 10000000000000000));
    BOOST_CHECK_EQUAL(100000000000000000, mastercore::calculateDesiredLTC(1000000000, 1000000000, 100000000000000000));
    BOOST_CHECK_EQUAL(1000000000000000000, mastercore::calculateDesiredLTC(1000000000, 1000000000, 1000000000000000000));

}

BOOST_AUTO_TEST_CASE(calculate_dexpurchase)
{

    // 1 LTC = 1 ALL
    int64_t amountOffered = 400000000; // 4 ALL
    int64_t amountDesired = 400000000; // 4 LTCs
    int64_t amountPaid = 300000000; // 3 LTC (left to offer)

    // checking : amountOffered * (amountPaid / amountDesired)
    BOOST_CHECK_EQUAL(300000000, mastercore::calculateDExPurchase(amountOffered, amountDesired, amountPaid));


    // 1 LTC = 1 ALL
    amountOffered = numeric_limits<int64_t>::max(); // 92,233,720,368.54775807 ALL
    amountDesired = 100000000; // 1 LTCs
    amountPaid = 100000000; // 1 LTC (left to offer)

    // checking : amountOffered * (amountPaid / amountDesired)
    BOOST_CHECK_EQUAL(9223372036854775807, mastercore::calculateDExPurchase(amountOffered, amountDesired, amountPaid));

    // Increasing amountOffered
    BOOST_CHECK_EQUAL(1000000000000000000, mastercore::calculateDExPurchase(1, 1000000000, 1000000000));
    BOOST_CHECK_EQUAL(100000000000000000, mastercore::calculateDExPurchase(10, 1000000000, 1000000000));
    BOOST_CHECK_EQUAL(10000000000000000, mastercore::calculateDExPurchase(100, 1000000000, 1000000000));
    BOOST_CHECK_EQUAL(1000000000000000, mastercore::calculateDExPurchase(1000, 1000000000, 1000000000));
    BOOST_CHECK_EQUAL(100000000000000, mastercore::calculateDExPurchase(10000, 1000000000, 1000000000));
    BOOST_CHECK_EQUAL(10000000000000, mastercore::calculateDExPurchase(100000, 1000000000, 1000000000));
    BOOST_CHECK_EQUAL(1000000000000, mastercore::calculateDExPurchase(1000000, 1000000000, 1000000000));
    BOOST_CHECK_EQUAL(100000000000, mastercore::calculateDExPurchase(10000000, 1000000000, 1000000000));
    BOOST_CHECK_EQUAL(10000000000, mastercore::calculateDExPurchase(100000000, 1000000000, 1000000000));
    BOOST_CHECK_EQUAL(1000000000, mastercore::calculateDExPurchase(1000000000, 1000000000, 1000000000));
    BOOST_CHECK_EQUAL(100000000, mastercore::calculateDExPurchase(10000000000, 1000000000, 1000000000));
    BOOST_CHECK_EQUAL(10000000, mastercore::calculateDExPurchase(100000000000, 1000000000, 1000000000));
    BOOST_CHECK_EQUAL(1000000, mastercore::calculateDExPurchase(1000000000000, 1000000000, 1000000000));
    BOOST_CHECK_EQUAL(100000, mastercore::calculateDExPurchase(10000000000000, 1000000000, 1000000000));
    BOOST_CHECK_EQUAL(10000, mastercore::calculateDExPurchase(100000000000000, 1000000000, 1000000000));
    BOOST_CHECK_EQUAL(1000, mastercore::calculateDExPurchase(1000000000000000, 1000000000, 1000000000));
    BOOST_CHECK_EQUAL(100, mastercore::calculateDExPurchase(10000000000000000, 1000000000, 1000000000));
    BOOST_CHECK_EQUAL(10, mastercore::calculateDExPurchase(100000000000000000, 1000000000, 1000000000));
    BOOST_CHECK_EQUAL(1, mastercore::calculateDExPurchase(1000000000000000000, 1000000000, 1000000000));

    // Increasing amountDesired
    BOOST_CHECK_EQUAL(1, mastercore::calculateDExPurchase(1000000000, 1, 1000000000));
    BOOST_CHECK_EQUAL(10, mastercore::calculateDExPurchase(1000000000, 10, 1000000000));
    BOOST_CHECK_EQUAL(100, mastercore::calculateDExPurchase(1000000000, 100, 1000000000));
    BOOST_CHECK_EQUAL(1000, mastercore::calculateDExPurchase(1000000000, 1000, 1000000000));
    BOOST_CHECK_EQUAL(10000, mastercore::calculateDExPurchase(1000000000, 10000, 1000000000));
    BOOST_CHECK_EQUAL(100000, mastercore::calculateDExPurchase(1000000000, 100000, 1000000000));
    BOOST_CHECK_EQUAL(1000000, mastercore::calculateDExPurchase(1000000000, 1000000, 1000000000));
    BOOST_CHECK_EQUAL(10000000, mastercore::calculateDExPurchase(1000000000, 10000000, 1000000000));
    BOOST_CHECK_EQUAL(100000000, mastercore::calculateDExPurchase(1000000000, 100000000, 1000000000));
    BOOST_CHECK_EQUAL(1000000000, mastercore::calculateDExPurchase(1000000000, 1000000000, 1000000000));
    BOOST_CHECK_EQUAL(10000000000, mastercore::calculateDExPurchase(1000000000, 10000000000, 1000000000));
    BOOST_CHECK_EQUAL(100000000000, mastercore::calculateDExPurchase(1000000000, 100000000000, 1000000000));
    BOOST_CHECK_EQUAL(1000000000000, mastercore::calculateDExPurchase(1000000000, 1000000000000, 1000000000));
    BOOST_CHECK_EQUAL(10000000000000, mastercore::calculateDExPurchase(1000000000, 10000000000000, 1000000000));
    BOOST_CHECK_EQUAL(100000000000000, mastercore::calculateDExPurchase(1000000000, 100000000000000, 1000000000));
    BOOST_CHECK_EQUAL(1000000000000000, mastercore::calculateDExPurchase(1000000000, 1000000000000000, 1000000000));
    BOOST_CHECK_EQUAL(10000000000000000, mastercore::calculateDExPurchase(1000000000, 10000000000000000, 1000000000));
    BOOST_CHECK_EQUAL(100000000000000000, mastercore::calculateDExPurchase(1000000000, 100000000000000000, 1000000000));
    BOOST_CHECK_EQUAL(1000000000000000000, mastercore::calculateDExPurchase(1000000000, 1000000000000000000, 1000000000));

    // Increasing amountPaid
    BOOST_CHECK_EQUAL(1, mastercore::calculateDExPurchase(1000000000, 1000000000, 1));
    BOOST_CHECK_EQUAL(10, mastercore::calculateDExPurchase(1000000000, 1000000000, 10));
    BOOST_CHECK_EQUAL(100, mastercore::calculateDExPurchase(1000000000, 1000000000, 100));
    BOOST_CHECK_EQUAL(1000, mastercore::calculateDExPurchase(1000000000, 1000000000, 1000));
    BOOST_CHECK_EQUAL(10000, mastercore::calculateDExPurchase(1000000000, 1000000000, 10000));
    BOOST_CHECK_EQUAL(100000, mastercore::calculateDExPurchase(1000000000, 1000000000, 100000));
    BOOST_CHECK_EQUAL(1000000, mastercore::calculateDExPurchase(1000000000, 1000000000, 1000000));
    BOOST_CHECK_EQUAL(10000000, mastercore::calculateDExPurchase(1000000000, 1000000000, 10000000));
    BOOST_CHECK_EQUAL(100000000, mastercore::calculateDExPurchase(1000000000, 1000000000, 100000000));
    BOOST_CHECK_EQUAL(1000000000, mastercore::calculateDExPurchase(1000000000, 1000000000, 1000000000));
    BOOST_CHECK_EQUAL(10000000000, mastercore::calculateDExPurchase(1000000000, 1000000000, 10000000000));
    BOOST_CHECK_EQUAL(100000000000, mastercore::calculateDExPurchase(1000000000, 1000000000, 100000000000));
    BOOST_CHECK_EQUAL(1000000000000, mastercore::calculateDExPurchase(1000000000, 1000000000, 1000000000000));
    BOOST_CHECK_EQUAL(10000000000000, mastercore::calculateDExPurchase(1000000000, 1000000000, 10000000000000));
    BOOST_CHECK_EQUAL(100000000000000, mastercore::calculateDExPurchase(1000000000, 1000000000, 100000000000000));
    BOOST_CHECK_EQUAL(1000000000000000, mastercore::calculateDExPurchase(1000000000, 1000000000, 1000000000000000));
    BOOST_CHECK_EQUAL(10000000000000000, mastercore::calculateDExPurchase(1000000000, 1000000000, 10000000000000000));
    BOOST_CHECK_EQUAL(100000000000000000, mastercore::calculateDExPurchase(1000000000, 1000000000, 100000000000000000));
    BOOST_CHECK_EQUAL(1000000000000000000, mastercore::calculateDExPurchase(1000000000, 1000000000, 1000000000000000000));

}

BOOST_AUTO_TEST_SUITE_END()
