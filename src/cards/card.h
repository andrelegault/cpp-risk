using namespace std;

// A Risk card type.
enum Card_Type { INFANTRY, ARTILLERY, CAVALRY };

/**
 * A Risk card with a Card_Type.
 */
struct Card {
    // The Card_Type.
    Card_Type card_type;
};