#ifndef __STORY__HPP__
#define __STORY__HPP__

#include <vector>
#include "constants.hpp"
#include "controls.hpp"
#include "codewords.hpp"
#include "items.hpp"
#include "skills.hpp"
#include "character.hpp"

namespace Choice
{
    enum class Type
    {
        NORMAL = 0, // No requirements
        ITEM,
        GET_ITEM,
        GIVE_ITEM,
        SKILL,
        CODEWORD,
        MONEY,
        LIFE,
        ANY_ITEM,
        ANY_SKILL,
        SKILL_ITEM,
        SKILL_ANY,
        SKILL_ALL,
        FIRE_WEAPON,
        LOSE_ITEM,
        LOSE_LIFE,
        LOSE_MONEY,
        GIVE_MONEY
    };

    class Base
    {
    public:
        const char *Text = NULL;

        Choice::Type Type = Choice::Type::NORMAL;

        Skill::Type Skill = Skill::Type::NONE;

        std::vector<Item::Type> Items = std::vector<Item::Type>();
        Item::Type Item = Item::Type::NONE;

        int Value = 0;

        int Destination = -1;

        Base(Choice::Type type, const char *text, int destination, Skill::Type skill, Item::Type item, int value)
        {
            Type = type;
            Text = text;
            Destination = destination;
            Item = item;
            Skill = skill;
            Value = value;
        }

        Base(const char *text, int destination, Skill::Type skill, Item::Type item)
        {
            Text = text;
            Destination = destination;
            Type = Choice::Type::SKILL_ITEM;
            Item = item;
            Skill = skill;
        }

        Base(const char *text, int destination, Item::Type item)
        {
            Text = text;
            Destination = destination;
            Type = Choice::Type::ITEM;
            Item = item;
        }

        Base(const char *text, int destination, Skill::Type skill)
        {
            Text = text;
            Destination = destination;
            Type = Choice::Type::SKILL;
            Skill = skill;
        }

        Base(const char *text, int destination, Choice::Type type, int value)
        {
            Text = text;
            Destination = destination;
            Type = type;
            Value = value;
        }

        Base(const char *text, int destination, Choice::Type type)
        {
            Text = text;
            Destination = destination;
            Type = type;
        }

        Base(const char *text, int destination, Choice::Type type, Item::Type item)
        {
            Text = text;
            Destination = destination;
            Type = type;
            Item = item;
        }

        Base(const char *text, int destination, Choice::Type type, std::vector<Item::Type> items)
        {
            Text = text;
            Destination = destination;
            Type = type;
            Items = items;
        }

        Base(const char *text, int destination, Choice::Type type, Skill::Type skill, std::vector<Item::Type> items)
        {
            Text = text;
            Destination = destination;
            Type = type;
            Skill = skill;
            Items = items;
        }
    };
} // namespace Choice

namespace Story
{
    enum class Type
    {
        NORMAL = 0,
        UNCERTAIN,
        GOOD,
        BAD
    };

    class Base
    {
    public:
        int ID = 0;

        const char *Text = NULL;
        const char *Title = NULL;
        const char *Bye = NULL;

        const char *Image = NULL;

        std::vector<Button> Controls = std::vector<Button>();
        std::vector<Choice::Base> Choices = std::vector<Choice::Base>();
        std::vector<std::pair<Item::Type, int>> Shop = std::vector<std::pair<Item::Type, int>>();

        // Player selects items to take up to a certain limit
        std::vector<Item::Type> Take = std::vector<Item::Type>();
        int TakeLimit = 0;

        Story::Type Type = Story::Type::NORMAL;

        // Handle background events
        virtual int Background(Character::Base &player) { return -1; };

        // Handle events before story branches
        virtual void Event(Character::Base &player){};

        // Jump to next section
        virtual int Continue(Character::Base &player) { return -1; };

        Base()
        {
        }

        Base(int id)
        {
            ID = id;
        }

        Base(int id, const char *text, Story::Type type)
        {
            ID = id;
            Text = text;
            type = type;
        }

        Base(const char *text, Story::Type type)
        {
            Text = text;
            type = type;
        }
    };

    int DONATION = 0;

    int BLESSING_WAR_GOD = false;

} // namespace Story

std::vector<Button> StandardControls()
{
    auto controls = std::vector<Button>();

    controls.push_back(Button(0, "images/up-arrow.png", 0, 1, 0, 1, (1 - Margin) * SCREEN_WIDTH - arrow_size, texty + border_space, Control::Type::SCROLL_UP));
    controls.push_back(Button(1, "images/down-arrow.png", 0, 2, 0, 2, (1 - Margin) * SCREEN_WIDTH - arrow_size, texty + text_bounds - arrow_size - border_space, Control::Type::SCROLL_DOWN));
    controls.push_back(Button(2, "images/map.png", 1, 3, 1, 2, startx, buttony, Control::Type::MAP));
    controls.push_back(Button(3, "images/disk.png", 2, 4, 1, 3, startx + gridsize, buttony, Control::Type::GAME));
    controls.push_back(Button(4, "images/user.png", 3, 5, 1, 4, startx + 2 * gridsize, buttony, Control::Type::CHARACTER));
    controls.push_back(Button(5, "images/items.png", 4, 6, 1, 5, startx + 3 * gridsize, buttony, Control::Type::USE));
    controls.push_back(Button(6, "images/next.png", 5, 7, 1, 6, startx + 4 * gridsize, buttony, Control::Type::NEXT));
    controls.push_back(Button(7, "images/exit.png", 6, 7, 1, 7, (1 - Margin) * SCREEN_WIDTH - buttonw, buttony, Control::Type::BACK));

    return controls;
}

std::vector<Button> ShopControls()
{
    auto controls = std::vector<Button>();

    controls.push_back(Button(0, "images/up-arrow.png", 0, 1, 0, 1, (1 - Margin) * SCREEN_WIDTH - arrow_size, texty + border_space, Control::Type::SCROLL_UP));
    controls.push_back(Button(1, "images/down-arrow.png", 0, 2, 0, 2, (1 - Margin) * SCREEN_WIDTH - arrow_size, texty + text_bounds - arrow_size - border_space, Control::Type::SCROLL_DOWN));
    controls.push_back(Button(2, "images/map.png", 1, 3, 1, 2, startx, buttony, Control::Type::MAP));
    controls.push_back(Button(3, "images/disk.png", 2, 4, 1, 3, startx + gridsize, buttony, Control::Type::GAME));
    controls.push_back(Button(4, "images/user.png", 3, 5, 1, 4, startx + 2 * gridsize, buttony, Control::Type::CHARACTER));
    controls.push_back(Button(5, "images/items.png", 4, 6, 1, 5, startx + 3 * gridsize, buttony, Control::Type::USE));
    controls.push_back(Button(6, "images/shop.png", 5, 7, 1, 6, startx + 4 * gridsize, buttony, Control::Type::SHOP));
    controls.push_back(Button(7, "images/next.png", 6, 8, 1, 7, startx + 5 * gridsize, buttony, Control::Type::NEXT));
    controls.push_back(Button(8, "images/exit.png", 7, 8, 1, 8, (1 - Margin) * SCREEN_WIDTH - buttonw, buttony, Control::Type::BACK));

    return controls;
}

class Prologue : public Story::Base
{
public:
    Prologue()
    {
        ID = 0;
        Title = "Necklace of Skulls: Prologue";
        Text = "Last night you dreamed you saw your brother again. He was walking through a desert, his sandals scuffing up plumes of sooty black sand from the low endless dunes. It seemed you were hurrying to catch him up, but the sand slipped away under your feet and you could make no headway up the slope. You heard your own voice call his name: \"Morning Star!\" But muffled by distance, the words went rolling off the sky unheeded.\n\nYou struggled on. Cresting the dune, you saw your brother standing close by, staring at something in his hands. Your heart thudded with relief as you stumbled through the dream towards him. But even as your hand reached out for his shoulder, a sense of dread was growing like a storm cloud to blot out any joy. You saw the object Morning Star was holding: an obsidian mirror. You leaned forward and gazed at the face of your brother reflected in the dark green glass.\n\nYour twin brother's face was the face of a skull.\n\nThe soothsayer nods as you finish recounting the dream. He plays idly with his carved stone prophecy-markers, pouring them from one hand to the other with a light rattling sound.\n\n\"Today is the day of Lamat,\" he says in his thin old voice. \"And the symbol of Lamat is the death's head. On this day, the morning-star has ended its cycle and will not be visible in the heavens for ninety days, when it will reappear as the evening-star. The meaning of the dream is therefore that in the absence of your brother it falls to you, Evening Star to fulfil his duties.\"\n\nYou cannot resist a smile, even though the ominous import of the dream weighs heavily on your soul. \"So it only concerns the importance of duty? I wonder if my clan elders have been speaking to you?\"\n\nThe soothsayer snorts and casts the prophecy-markers back into his bag with a pretence of indignation, but he has too good a heart to overlook your concern for your brother. Turning at the door, he adds, \"The King gave Morning Star a great honour when he made him his ambassador. But it is no less honourable to stay at home and help with the affairs of one's clan. You are young, Evening Star; your chance for glory will come.\"\n\n\"Do the prophecy-makers also tell you that?\"\n\nHe rattles the bag. \"These? They're just for show; it's the two old stones on either side of my nose that tell me everything I need to know about the future!\" He points to his eyes and hobbles out in a gale of wheezing laughter.\n\nYou lean back, feeling the cool of the stone wall press against your bare shoulders. The soothsayer intended to set your mind at rest, but you have shared a bond with Morning Star since the two of you were born. To be troubled by such a dream is not, you feel sure, a mere quirk of the imagination. Somehow you sense that something terrible has befallen your brother.\n\nYou are still brooding an hour later when a servant comes scurrying into the room. \"There is news of Lord Morning Star's expedition...\" he begins, almost too frightened of your reaction to blurt out the words.\n\nYou are on your feet in an instant. \"What news?\"\n\nThe servant bows. \"The Council of Nobles is holding an emergency session. The rumour... I have heard a rumour that only a single member of the expedition returned alive.\"\n\nPausing only to draw on your cloak, you hurry outside and head along the street towards the city centre. All around you sprawl the tall thatched roofs of the city, spreading out towards the distant fields. Each clan or group of families has its own dwellings of stone or mud-brick, according to status. These rest upon raised platforms above the level of the street, their height determined again by status. But not even the most exalted noble has a home to match the grand dwellings of the gods, which you now see towering ahead of you atop their immense pyramids. They shimmer with the colours of fresh blood and polished bone in the noonday sunlight, covered with demonic carvings which stare endlessly down across the city of Koba.\n\nThe central plaza of the city is a blaze of white stone in the sunshine. Quickening your step, you approach the amphitheatre where the Council of Nobles is meeting. As you step under the arch of the entrance, your way is barred by two burly warriors of the King's guard, each armed with an obsidian-edged sword. \"You may not enter.\"\n\n\"I am Evening Star. The ambassador is my brother. Has he returned to Koba?\"\n\nOne of them peers at you, recognition trickling like cold honey into his gaze. \"I know you now. Morning Star has not returned, no.\"\n\nThe other says, \"Look, I suppose you'd better go in. One of the ambassador's retinue came back this morning. He's telling the Council what happened.\"\n\nYou walk in to the amphitheatre and numbly find a seat. You can hardly take in the guard's words; they sit like stones in your head, impossible to accept. Can it be true? Your twin brother -- dead?\n\nA man you recognize as one of Morning Star's veteran warriors stands in the centre of the amphitheatre, giving his report. The seats on either side are crowded with the lords and ladies of Koba, each face a picture of grave deliberation. At the far end is the King himself, resplendent in a turquoise mantle of quetzal feathers, his throne carved to resemble the open jaws of some titanic monster on whose tongue he seems to sit like the very decree of the gods.\n\n\"...arrived at the Great City,\" the veteran is saying. \"We found it ransacked -- the temples torn down, whole palaces burned. Some poor wretches still live there, eking out a stark existence in the ruins, but it is like the carcass of a beast who lies with a death-wound. Whenever we asked how this destruction had come about, we received the same reply: werewolves from the land of the dead, beyond the west, had descended from the desert and slain all the Great City's defenders in a single night of carnage.\"\n\nThere is a murmuring at this. The Great City had endured for centuries before Koba was even built. The King raises his hand for silence. \"What was Lord Morning Star's decision when he heard this?\"\n\n\"Majesty, he led us into the desert. He believed it his duty to uncover the truth of the matter and report it to you. After many days of trekking almost all our water was used up. We had faced monsters along the way, and many of us bore grievous injuries. Then we came to a place like a royal palace, but entirely deserted except for dogs and owls. We camped outside the walls there, and on the next day Morning Star told us he had dreamed of a sorcerer called Necklace of Skulls who dwelt within the palace. He said he would enter and find out if this sorcerer had sent the werewolves to destroy the Great City. We watched him enter the portals of the palace, and we waited for his return for eight days, but he did not emerge. Then we began the long march back here to Koba, but sickness and the creatures of the desert gradually took their toll, and I alone remain to tell the tale.\"\n\nThe King rises to his feet. \"Morning Star must be considered slain by this sorcerer. His mission shall not be recorded as a failure, since he died attempting to carry out his duty. Prayers shall be said for the safe journey of his soul through the underworld. This meeting is ended.\"\n\nThe others file out in groups, heads bent together in urgent debate. For most of them the veteran's report carried that special thrill of a distant alarm. A great but far-off city reduced to ruin; a disaster from halfway across the world. Cataclysmic news, but an event comfortingly remote from the day to day affairs at home. A matter for the noblemen to worry over when they sit with their cigars at night. The reverberations of a toppling temple in the Great City will be heard here in Koba as no more than the droning discussions of old men.\n\nFor you it is different. Left alone in the amphitheatre, you sit like a figure of clay, eerily detached from your own turbulent emotions. Fractured images and words whirl through your stunned brain. Morning Star is dead. Your twin brother, lost for ever...\n\nA single sudden thought of burning clarity impels you to your feet. In that instant you seem to see your destiny unrolling in front of you like along straight carpet. You turn your face to the west, eyes narrowing in the glare of the declining sun.\n\nYour brother might not be dead. There is only one place you can learn the truth.\n\nYou must journey to the western desert, to the palace of the sorcerer called Necklace of Skulls.";

        Image = "images/filler1.png";

        Choices.clear();
        Controls = StandardControls();
    }

    int Continue(Character::Base &player) { return 1; };
};

class Story001 : public Story::Base
{
public:
    Story001()
    {
        ID = 1;
        Text = "Seeking an audience with the Matriarch of your clan, you are shown into a narrow steep-vaulted hall. Sunlight burns through the high window slits to leave hovering blocks of dazzling yellow light on the whitewashed wall, but the room is cool.\n\nThe Matriarch sits cross-legged on a stone bench at the end of the room, below a large painted glyph which is the symbol of the clan. A stout woman in late middle-age, she has a soft and even jolly appearance which is belied by the look of stern contemplation in her eyes. The beads sewn across here cotton mantle make a rustling sound as she waves you towards a straw mat. You bow in greeting before sitting, and a servant brings you a cup of frothy peppered cocoa.\n\nThe Matriarch fixes you with her glass-bead gaze. \"Evening Star, I understand you wish to leave Koba and travel in search of your brother.\"\n\n\"I must learn what has happened to him, my lady. If he is alive, perhaps I can rescue him; if dead, it is my duty to avenge him.\"\n\nThe Matriarch folds her fat jade-ringed fingers and rests her chin on them, watching you as though weighing the worth of your soul. \"You speak of duty,\" she says. \"Have you no duty to your clan here in Koba? Does honour demand that we lose another scion in pursuit of a hopeless quest?\"\n\nYou sip cocoa while considering your next words carefully.";
        Image = "images/filler1.png";

        Choices.clear();
        Choices.push_back(Choice::Base("Reply that the life of your brother is more important than your duty to the clan", 24, Choice::Type::NORMAL));
        Choices.push_back(Choice::Base("...that on the contrary, clan honour demands that you go", 47, Choice::Type::NORMAL));
        Choices.push_back(Choice::Base("Say nothing", 70, Choice::Type::NORMAL));

        Controls = StandardControls();
    }
};

class Story002 : public Story::Base
{
public:
    Story002()
    {
        ID = 2;
        Text = "The priest accepts your offering, glancing at the cacao with a slight smile before dropping them into his belt pouch. \"I consulted the oracle before you came, and here is the advice given. Travel first to the town of Balak on the northern coast. Do not put to sea at once; wait a week before you set sail. You might be tempted to put in at the Isle of the Iguana, but be warned that only an accomplished seafarer should venture out of sight of land.\"\n\n\"What about the desert?\" you ask.\n\n\"I told you already, buy a waterskin. And there's one other thing that the oracle revealed. You must seek the blood that is like sap.\"\n\n\"Enigmatic,\" you say, considering this, \"but I suppose that's the way of oracles.\"\n\nThe priest beams happily. \"Just so. Before you go, can I interest you in a companion for the journey?\" He claps his hands, and a novitiate priest steps out from around the side side of the shrine carrying a bird on his arm.\n\nYou look from the high priest to the bird to the novitiate. \"A companion, you say? Do you mean this lad, or the owl?\"\n\n\"The owl. The lad is needed here to trim the votive lamps in the shrine each evening. But I think you will find the owl a personable companion. Just keep it fed on small rodents and the like. The price is three cacao.\"\n\nYou peer closely at the owl. \"Isn't the owl the symbol of death? Additionally, unless I am much mistaken this bird has only one leg.\"\n\nThe high priest shrugs. \"Call it two cacao, then. And let me set your mind at rest: the owl is also a symbol of night and the moon.\"";
        Image = "images/filler1.png";

        Choices.clear();

        Controls = ShopControls();

        Shop = {{Item::Type::OWL, 2}};
    }

    int Continue(Character::Base &player) { return 93; }
};

class Story024 : public Story::Base
{
public:
    Story024()
    {
        ID = 24;
        Text = "Convinced though you are by the strength of this argument, the Matriarch seems unimpressed. \"I wonder what you will find?\" she muses. \"The truth as to Morning Star's fate, perhaps. But also you may discover a deeper truth.\"\n\nShe turns aside to gaze up at the shimmering sunbeams slanting through the windows, showing her profile in an expression of disdain. \"You are useless to the clan until you learn wisdom, Evening Star. Depart whenever you wish.\"\n\nShe says nothing more. Awkwardly, you get to your feet and retreat from the room with a hesitant bow.";
        Image = "images/filler1.png";

        Choices.clear();
        Controls = StandardControls();
    }

    int Continue(Character::Base &player) { return 93; }
};

class Story025 : public Story::Base
{
public:
    Story025()
    {
        ID = 25;
        Text = "The causeway to Yashuna is an arrow-straight road of packed limestone raised on stone blocks above the level of the countryside. As you walk, you scan the swaying fields of maize, the orchards and ranks of cotton plants that stretch off as far as the eye can see. Low stone walls mark the irrigation channels that ensure as much water as possible reaches the crops at this arid time of year. Peasant dwellings are scattered here and there across the countryside: oval single-storey buildings with sharply peaked roofs of dry thatch. It makes you thirsty just to watch the peasants at their back-breaking work, gathering cotton in long sacks under the sweltering sun.\n\nA dusty grove of papaya trees overhangs the causeway. Your mouth waters as you look at them. Surely on one would mind if you took just one papaya? As you reach up to pick one of the fruits, there is a sudden flurry of movement from the bole of the tree. You go rigid, and a thrill of clammy fear chills you despite the heat of the day. Poised atop the fruit is a tarantula! Its huge black forelimbs are resting on your fingers, and you can see the wet coating of venom on its fangs.";
        Image = "images/filler1.png";

        Choices.clear();
        Choices.push_back(Choice::Base(Skill::WILDERNESS_LORE.Name, 48, Skill::Type::WILDERNESS_LORE));
        Choices.push_back(Choice::Base("Jerk your hand away quickly", 71, Choice::Type::NORMAL));
        Choices.push_back(Choice::Base("Slowly reach around with your other hand to seize the tarantula from behind", 94, Choice::Type::NORMAL));

        Controls = StandardControls();
    }
};

class Story047 : public Story::Base
{
public:
    Story047()
    {
        ID = 47;
        Text = "\"What would others think of our clan,\" you assert, \"if we meekly ignored the loss of my brother? Honour is like the sun: it cannot hide its face.\"\n\nThe Matriarch thrusts her head forward and stares at you along the great hook of her nose. Perched thus on her stone seat, she reminds you of a fat owl watching a mouse. You begin to fear you have offended her with your frank answer, but then to your relief she gives a rumble of approving laughter. \"Well said, young Evening Star. How like your brother you are -- and both of you like your late father, always brimming over with impatient courage.\"\n\nYou set down your cup. \"Then have I your leave to go, my lady?\"\n\nShe nods. \"Yes, but since your determination glorifies the clan, I feel that the clan should give you assistance in this quest. Consider what help you need most, Evening Star. I could arrange for you to have an audience with one of our high priests, and you could seek their advice. Or I could allow you to equip yourself with the clan's special ancestral treasures. Or would you prefer a companion on your quest?\"";
        Image = "images/filler1.png";

        Choices.clear();
        Choices.push_back(Choice::Base("Request a meeting with one of the high priests", 116, Choice::Type::NORMAL));
        Choices.push_back(Choice::Base("Ask to see the ancestral treasures of the clan", 138, Choice::Type::NORMAL));
        Choices.push_back(Choice::Base("You think a companion would be useful", 162, Choice::Type::NORMAL));

        Controls = StandardControls();
    }
};

class Story048 : public Story::Base
{
public:
    Story048()
    {
        ID = 48;
        Text = "You know that the spider must be torpid from the heat. Tarantulas are night hunters. It is unlikely to bite if you jerk your hand away, and even if it did the venom is little worse than a wasp sting. Touching it would be far more unpleasant, since the bristles inject a powerful irritant.\n\nThe tarantula sleepily probes your fingers with its limbs. You snatch your hand back out of its clutches. Its only reaction is to slowly curl back into the shade of the papaya fruit. You breathe a sigh of relief and step back to the middle of the causeway.\n\n\"Hey there! What're you doing?\"\n\nYou turn to see an old peasant coming through the dusty orchard towards you.";
        Image = "images/filler1.png";

        Choices.clear();
        Choices.push_back(Choice::Base("Talk to him", 117, Choice::Type::NORMAL));
        Choices.push_back(Choice::Base("Hurry off before he gets here", 163, Choice::Type::NORMAL));

        Controls = StandardControls();
    }

    void Event(Character::Base &player)
    {
        if (Character::VERIFY_SKILL(player, Skill::Type::ETIQUETTE))
        {
            Choices[0].Destination = 139;
            Choices[0].Type = Choice::Type::SKILL;
            Choices[0].Skill = Skill::Type::ETIQUETTE;
        }
        else
        {
            Choices[0].Destination = 117;
            Choices[0].Type = Choice::Type::NORMAL;
            Choices[0].Skill = Skill::Type::NONE;
        }
    }
};

class Story070 : public Story::Base
{
public:
    Story070()
    {
        ID = 70;
        Text = "\"I can give no easy answer, my lady,\" you tell the Matriarch. \"I do not wish to shirk my duty to the clan that has nurtured me, but neither can I ignore the demands of my heart. I must go in search of my brother, since I cannot rest until I know whether he is alive or dead.\"\n\nShe heaves a deep sigh, more of resignation than disapproval. \"I know you could not be dissuaded,\" she says. \"You have your late father's impetuosity. Morning Star shared that same quality. It is the mark of a hero -- but beware, Evening Star, for it can also get you killed.\"\n\n\"I understand. I have your permission to undertake this quest, then?\"\n\n\"You have.\" She produces a letter and hands it to you. \"Take this to the town of Balak on the northern coast. Ask there for a girl named Midnight Bloom. She is a distant cousin of yours. Present her with this letter, which will introduce you and request her assistance in your quest.\"\n\n\"How can she assist me?\" you ask, taking the LETTER OF INTRODUCTION.\n\n\"She is skilled in coastal trade, and will convey you by ship to Tahil. May the gods watch over you, Evening Star.\"\n\nYou rise and bow, as you leave, your heart is full of excitement.";
        Image = "images/filler1.png";

        Choices.clear();
        Controls = StandardControls();
    }

    int Continue(Character::Base &player) { return 93; }

    void Event(Character::Base &player)
    {
        Character::GET_UNIQUE_ITEMS(player, {Item::Type::LETTER_OF_INTRODUCTION});
    }
};

class Story071 : public Story::Base
{
public:
    Story071()
    {
        ID = 71;
        Text = "The tarantula drowsily probes your fingers with its bristly limbs. Its movement evokes a feeling of fascination and revulsion -- you can well imagine how a mouse might feel as one of these hairy monsters came rushing out of the dark of night to seize it! You snatch your hand back quickly. The tarantula's only reaction is to slowly curl back into the shade of the papaya fruit. You breathe a sigh of relief and step back out from under the tree.\n\n\"Hey, you there! What are you doing?\"\n\nYou turn to see an old peasant coming through the dusty orchard towards the causeway.";
        Image = "images/filler1.png";

        Choices.clear();
        Choices.push_back(Choice::Base("Talk to him", 117, Choice::Type::NORMAL));
        Choices.push_back(Choice::Base("Hurry off before he gets here", 163, Choice::Type::NORMAL));

        Controls = StandardControls();
    }

    void Event(Character::Base &player)
    {
        if (Character::VERIFY_SKILL(player, Skill::Type::ETIQUETTE))
        {
            Choices[0].Destination = 139;
            Choices[0].Type = Choice::Type::SKILL;
            Choices[0].Skill = Skill::Type::ETIQUETTE;
        }
        else
        {
            Choices[0].Destination = 117;
            Choices[0].Type = Choice::Type::NORMAL;
            Choices[0].Skill = Skill::Type::NONE;
        }
    }
};

class Story093 : public Story::Base
{
public:
    Story093()
    {
        ID = 93;
        Text = "Realizing there are things you will need on your travels, you head to the market. Here, under a long colonnade festooned with coloured rugs, you can usually find almost anything. Unfortunately it is now late afternoon and many of the traders have packed up their wares and gone home, driven off by the waves of heat rising from the adjacent plaza.\n\nMaking your way along the colonnade, you identify the different goods at a glance according to the colours of the rugs. Green indicates sellers of maize, while yellow and red are used for other foodstuffs. Black is the colour of stone or glass items, with the addition of grey frets signifying weaponry. Wooden products are set out on ochre cloth, and white is reserved for clay pottery.\n\nSoon you have found a few items which might prove useful. You count the cacao in your money-pouch while considering which to buy.";
        Image = "images/filler1.png";

        Choices.clear();
        Controls = ShopControls();

        Shop = {
            {Item::Type::WATERSKIN, 2},
            {Item::Type::ROPE, 3},
            {Item::Type::FIREBRAND, 2},
            {Item::Type::POT_OF_DYE, 2},
            {Item::Type::CHILLI_PEPPERS, 1}};
    }

    int Continue(Character::Base &player) { return 389; }
};

class Story094 : public Story::Base
{
public:
    std::string PreText = "";

    Story094()
    {
        ID = 94;

        Image = "images/filler1.png";

        Choices.clear();
        Choices.push_back(Choice::Base("Talk to him", 117, Choice::Type::NORMAL));
        Choices.push_back(Choice::Base("Hurry off before he gets here", 163, Choice::Type::NORMAL));

        Controls = StandardControls();
    }

    void Event(Character::Base &player)
    {
        PreText = "The spider's bristly limbs send a shiver through you as they slowly probe your outstretched hand. It takes every shred of nerve to remain motionless while you carefully reach around behind it with your other hand. Its multiple eyes gleam horribly, full of the ruthless intensity of the predator. It looks like a demon carved from polished mahogany, more nightmarish than any image on the walls of the Temple of Death.\n\nAs you take hold of it, it starts to twitch its legs furiously. With a sob of revulsion, you hurl it away. It falls with an audible thud somewhere off among the trees, but then a stab of pain convulses your hand. Did it bite you after all? You have to prise your fingers apart, but instead of a bite you find dozens of tiny pinpricks all over your palm. The tarantula's bristles were razor-sharp, and seem to have injected a stinging chemical into your skin.\n\nYou LOSE 1 Life Point.\n";

        player.Life -= 1;

        if (player.Life > 0)
        {
            if (Character::VERIFY_SKILL(player, Skill::Type::ETIQUETTE))
            {
                Choices[0].Destination = 139;
                Choices[0].Type = Choice::Type::SKILL;
                Choices[0].Skill = Skill::Type::ETIQUETTE;
            }
            else
            {
                Choices[0].Destination = 117;
                Choices[0].Type = Choice::Type::NORMAL;
                Choices[0].Skill = Skill::Type::NONE;
            }

            PreText += "\n\"Hey, you there! What are you doing?\"\n\nYou look up to see an old peasant hurrying through the dusty orchard towards the causeway.";

            Text = PreText.c_str();
        }
        else
        {
            Text = PreText.c_str();
        }
    }
};

class Story096 : public Story::Base
{
public:
    Story096()
    {
        ID = 96;
        Text = "They succeed in dislodging several fat plums without disturbing any spiders. You watch as they squabble happily over the distribution of their spoils. Apparently you were just unlucky in finding a tarantula in the fruit you tried to pick, but the incident has deadened your appetite and you continue on your way without stopping to collect any of the plums yourself.";

        Image = "images/filler1.png";

        Choices.clear();
        Controls = StandardControls();
    }

    int Continue(Character::Base &player) { return 350; };
};

class Story103 : public Story::Base
{
public:
    Story103()
    {
        ID = 103;
        Text = "\"Wait!\"\n\nAll eyes turn in your direction. The high priest scowls, \"Who is this outsider who dares to interrupt the sacred rite?\"\n\n\"Release them,\" you say, ignoring him. \"I shall carry your petition into the underworld.\"\n\nThe priest strides over, pressing his face inches from yours with a look of black fury. \"You? Why should I let you undertake this journey?\"\n\n\"Because I was sent here by a god.\"\n\nHe has no answer to that. For a moment his mouth works silently, ready to frame a protest, but he has already seen the light of truth in your eyes. Stepping back, he gives a nod and the young couple are set free.";
        Image = "images/filler1.png";

        Choices.clear();
        Choices.push_back(Choice::Base("Jump into the sacred well", 327, Choice::Type::NORMAL));
        Choices.push_back(Choice::Base("Cast a protective enchantment first (SPELLS)", 304, Choice::Type::SKILL_ANY, Skill::Type::SPELLS, {Item::Type::MAGIC_WAND, Item::Type::JADE_SWORD}));

        Controls = StandardControls();
    }
};

class Story116 : public Story::Base
{
public:
    Story116()
    {
        ID = 116;
        Text = "Which temple do you wish to visit?";
        Image = "images/filler1.png";

        Choices.clear();
        Choices.push_back(Choice::Base("Visit the temple of the War God", 231, Choice::Type::NORMAL));
        Choices.push_back(Choice::Base("Vist the temple of the Moon Goddess", 254, Choice::Type::NORMAL));
        Choices.push_back(Choice::Base("Visit the temple of the Death God", 277, Choice::Type::NORMAL));
        Choices.push_back(Choice::Base("You do not think any of the priests will be of much help,and would ask the Matriarch to let you have some of the clan treasures", 138, Choice::Type::NORMAL));

        Controls = StandardControls();
    }
};

class Story117 : public Story::Base
{
public:
    Story117()
    {
        ID = 117;
        Text = "\"I was very nearly bitten by a tarantula!\" you tell the peasant as he comes up to the side of the causeway.\n\nHe mops at his brow. \"They enjoy the cool and moisture under the bunches of fruit,\" he remarks. \"Sometimes I wish I too were a tarantula, and not a poor farmer who must toil in this sweltering heat.\"\n\nYou smile, familiar with the customary grumblings of peasants. \"Let us hope the rains will be abundant this year,\" you say by way of conversation. \"The crops grow worse because of the drought.\"\n\n\"In Yashuna the priests are holding a ceremony in honour of the Rain God,\" he says, nodding. Is it your imagination, or does a craft look come into his eye as he adds: \"My eldest son was going to attend the ceremony, but I need him to help me in the fields. Perhaps you would like to go in his place?\"\n\n\"I presume the priests would not appreciate all and sundry poking their nose into such sacred rituals.\"\n\n\"Quite so,\" he says. \"But I have here a jade bracelet which my son was told to wear. It authorizes him to take an intimate role in the proceedings. I could sell it to you for a cacao or two.\"\n\nYou study the bracelet he is holding out. It is in the shape of a water serpent with the glyph of the Rain God on its triangular head. \"In all candour, this is worth rather more than the sum mentioned,\" you reply cautiously.\n\nHe shrugs. \"I would be happy for any money at all in these hard times. Is it a deal, or not?";
        Image = "images/filler1.png";

        Choices.clear();
        Controls = ShopControls();

        Shop = {{Item::Type::SERPENT_BRACELET, 1}};

        Bye = "Bidding the peasant good.day, you set off once more towards Yashuna";
    }

    int Continue(Character::Base &player) { return 163; }
};

class Story120 : public Story::Base
{
public:
    Story120()
    {
        ID = 120;
        Text = "Following the crowds, you pass under an archway at the north edge of the city. People go milling past and you find yourself carried by the surge of bodies along a plaster-paved causeway that leads through light woodland. You are surprised to see no buildings on this side of the city. The crowds are all eagerly discussing some great spectacle that lies in store, and you catch snatches of conversation as you are borne along. \"Now the Rain God will no longer forsake us,\" says one man. A woman who is carrying two squalling brats shouts back over her shoulder: \"Just so long as the sacrifice pleases him! Don't forget that.\"\n\nThen the trees give way to an immense clearing. At first you cannot tell what lies ahead, but by pushing your way forward you reach the front of the crowd. Your breath escapes from your lungs in a gasp of awe. The clearing is formed by a gigantic hole in the ground. It looks as though the crust of the earth simply crumbled away to real an entrance into the underworld. The sinkhole is more than twenty metres deep and even a strong man could never hope to cast a spear right across to the far side. The sides of the pit are raw limestone clothed in a dry tangled mass of roots and creepers, dropping right down to the murky lake that occupies the bottom of this vast cavernous gulf.\n\n\"What is it?\" you ask a priest standing beside you.\n\nWhen he answers, you discover that your first wild impression was correct. \"This is the sacred well of Yashuna,\" he replies in a stately voice. \"It is the mouth of the underworld.\"";
        Image = "images/filler1.png";

        Choices.clear();

        Controls = StandardControls();
    }

    int Continue(Character::Base &player)
    {
        if (Character::VERIFY_ITEM(player, Item::Type::SERPENT_BRACELET))
        {
            return 257;
        }
        else
        {
            return 234;
        }
    }
};

class Story138 : public Story::Base
{
public:
    Story138()
    {
        ID = 138;
        Text = "The Matriarch speaks to a servant, who goes bustling out and returns shortly leading two slaves bearing a large wooden chest. This is set down in front of the Matriarch's seat and the two slaves are ushered outside before it can be opened. The Matriarch beckons you over. \"These,\" she says, delving into the interior of the chest, \"are the treasures of our ancestors.\"\n\nA golden figuring catches your eye. It is in the form of a muscular naked man with an elongated forehead. \"What is this?\" you ask.\n\n\"The MAN OF GOLD -- most ancient of all our treasures. It is said that in the earliest days of the world, the gods experimented with various substances to create life. One of the lesser gods tried using gold, but because it was so scarce he could only make a small human.\"\n\nYou lift the the MAN OF GOLD with a sense of awe. \"Is it alive, then?\"\n\n\"If you hold it in your hands long enough, it will come to life through your body's warmth. Then it will serve you with great strength and skill -- but only once.\"\n\n\"Only once?\" you ask. \"If it only works once, how does anyone know this?\"\n\nThe Matriarch responds with a sly wink. \"You have to trust your elders sometimes, Evening Star. Now, do you want the MAN OF GOLD or would you rather take a look at the other treasures?\"";
        Image = "images/filler1.png";

        Choices.clear();
        Choices.push_back(Choice::Base("Take the MAN OF GOLD", 93, Choice::Type::GET_ITEM, Item::Type::MAN_OF_GOLD));
        Choices.push_back(Choice::Base("Choose from the rest of the treasures", 185, Choice::Type::NORMAL));

        Controls = StandardControls();
    }
};

class Story139 : public Story::Base
{
public:
    Story139()
    {
        ID = 139;
        Text = "Your bearing and accent immediately mark you as a member of the nobility. The peasant stands watching you with a sullen expression. \"In these times of drought my fruit is precious to me,\" he says. \"But I will sell you a PAPAYA for two cacao.\"\n\n\"Your fruit is infested with poisonous spiders,\" you reply proudly. I am doubtful whether it is worth the risk of picking it, drought or not.\"\n\nHe compresses his lips, biting back an angry retort out of deference to your status. \"One cacao, then,\" he says.";
        Image = "images/filler1.png";

        Choices.clear();
        Controls = ShopControls();

        Bye = "Bidding the peasant a curt good-day, you continue along the causeway towards Yashuna";

        Shop = {{Item::Type::PAPAYA, 1}};
    }

    int Continue(Character::Base &player) { return 163; }
};

class Story142 : public Story::Base
{
public:
    Story142()
    {
        ID = 142;

        Text = "You are surprised to find the market almost deserted. Contrary to your assumption, most of the populace are not headed here, but are streaming to the causeway that leads north out of the city.\n\nThe stalls are set up under awnings whose cool shade is welcoming after the dusty heat of the road.\n\nYou stand back and examine the wares on offer. The traders are doing so little business that you should have the chance of some real bargains.\n\nYou make a show of strolling casually past a number of stalls, careful not to give any sign of interest in the items you want most. This will help you when the haggling starts.\n\nYou can check out what they have on sale.";

        Image = "images/filler1.png";

        Choices.clear();

        Controls = ShopControls();

        Shop = {
            {Item::Type::WATERSKIN, 1},
            {Item::Type::ROPE, 2},
            {Item::Type::TERRACOTTA_EFFIGY, 3},
            {Item::Type::BLOWGUN, 3},
            {Item::Type::INCENSE, 3}};
    }

    int Continue(Character::Base &player) { return 188; }
};

class Story162 : public Story::Base
{
public:
    Story162()
    {
        ID = 162;
        Text = "The Matriarch hands you a letter. \"Present this to Midnight Bloom, a distant cousin of yours who lives in the town of Balak on the northern coast,\" she explains. \"Midnight Bloom is an experienced seafarer, having traded the clan's goods with the distant city of Tahil for several years, and can arrange a passage for you there. Once in Tahil, you are halfway to your goal.\"\n\nYou take the LETTER OF INTRODUCTION. Rising to your feet, you bow to the Matriarch. \"I shall strive always to conduct myself with honour during my quest,\" you say.\n\n\"See that you do,\" she replies. \"You wear the clan's honour on your shoulders.\" As you reach the door, she calls after you: \"Oh, and Evening Star --\"\n\nYou turn. \"My lady?\"\n\n\"Good luck.\" She gives you one of her rare smiles -- a momentary crack in the sober mask of clan authority -- and waves you out into the bright sunshine.";
        Image = "images/filler1.png";

        Choices.clear();
        Controls = StandardControls();
    }

    int Continue(Character::Base &player) { return 93; }

    void Event(Character::Base &player)
    {
        Character::GET_UNIQUE_ITEMS(player, {Item::Type::LETTER_OF_INTRODUCTION});
    }
};

class Story163 : public Story::Base
{
public:
    Story163()
    {
        ID = 163;
        Text = "Travelling on, you see a group of small children gazing longingly at the fruit growing in the orchard beside the causeway. One of them finds a stick and goes over to prod at a bunch of juicy plums. In the light of your recent experience, you wonder if they might be in danger from tarantulas.";
        Image = "images/filler1.png";

        Choices.clear();
        Choices.push_back(Choice::Base("Give some food from your own pack (MAIZE CAKES)", 186, Choice::Type::GIVE_ITEM, Item::Type::MAIZE_CAKES));
        Choices.push_back(Choice::Base("Give some food from your own pack (PAPAYA)", 186, Choice::Type::GIVE_ITEM, Item::Type::PAPAYA));
        Choices.push_back(Choice::Base("Stand by and watch them pick the fruit", 96, Choice::Type::NORMAL));

        Controls = StandardControls();
    }
};

class Story165 : public Story::Base
{
public:
    Story165()
    {
        ID = 165;
        Text = "The causeway soon peters out and you leave the fields and orchards far behind. Light woodland begins to be replaced by the luxuriant foliage of the thick forest. After a few days you find yourself walking through deep leaf-rooted glades. Rainfall is more plentiful here than in the arid northern peninsula that is your homeland, and you are startled by the resulting profusion of vegetation and wildlife. For several days you subsist well enough on a diet of wild plums, avocados and breadnuts, but increasingly you find the plants of the region to be unfamiliar and you are no longer certain what is safe to eat.\n\nArriving at a wooden house, you introduce yourself the family living there. The man is a hunter who tells you that he formerly farmed a small plot near Yashuna. \"But lately the rains have been unreliable,\" he adds. \"At last I decided to bring my family south where the land is more bountiful.\"\n\n\"Are there no dangers to living close to the forest?\" you ask, casting a wary eye at the forbidding gloom between the tall tree-trunks.\n\nHe nods and draws deeply on his pipe. \"Many! Apart from supernatural creatures such as the spiteful stabai and the strangler beast, there are also jungle people who will brook no outsiders in their territory. They claim to be the guardians of the World Tree, which supports the sky itself.\"\n\nThe hunter asks no payment for the food and hospitality he gives you but you feel obliged to offer him a cacao anyway. Then, bidding these kind people farewell, you set off west towards the great city of Nachan.";
        Image = "images/filler1.png";

        Choices.clear();

        Controls = StandardControls();
    }

    int Continue(Character::Base &player) { return 209; }

    void Event(Character::Base &player)
    {
        player.Money -= 1;
    }
};

class Story185 : public Story::Base
{
public:
    Story185()
    {
        ID = 185;
        Text = "\"I cannot take this,\" you decide, replacing the golden statuette in the chest. \"It is too precious; the clan might one day need to use it.\"\n\n\"Well said!\" declares the Matriarch, her eyes almost vanishing in her plump face as she beams with satisfaction at your answer. \"The treasures that remain are less potent in their magic, but may also prove useful.\"\n\nThere are three other items. The first is a small mirror of dark green glass with a powerful spell inscribed around the rim. \"It can be used to see into the future,\" the Matriarch tells you. \"As for this next item --\" she holds up a sealed jar \"-- it contains a magic drink concocted by your great-grandfather which is capable of healing grievous wounds.\"\n\n\"And what of this?\" you ask, taking out a sword of sharpened green jade adorned with tiny glyphs.\n\n\"That also belonged to your great-grandfather. It served him both as a weapon of war and as a tool of his sorcery.\"";
        Image = "images/filler1.png";

        Choices.clear();
        Controls = StandardControls();

        Take = {{Item::Type::GREEN_MIRROR, Item::Type::MAGIC_POTION, Item::Type::JADE_SWORD}};

        TakeLimit = 2;
    }

    int Continue(Character::Base &player) { return 208; }
};

class Story186 : public Story::Base
{
public:
    Story186()
    {
        ID = 186;
        Text = "The children accept your gift with a look of open-mouthed astonishment before darting shyly off through the orchard. Pleased with yourself at your good deed, you whistle a jaunty tune as you continue towards Yashuna.";

        Image = "images/filler1.png";

        Choices.clear();

        Controls = StandardControls();
    }

    int Continue(Character::Base &player) { return 350; };
};

class Story188 : public Story::Base
{
public:
    Story188()
    {
        ID = 188;
        Text = "As you leave the market, a tall fellow emerges from the portico bordering on the temple plaza and stands surveying the marketplace. He is carrying pots that mark him as a fisherman, presumably from one of the coastal towns to the north. \"Huh!\" he mutters, half to himself. \"Is there no demand for good fish these days?\"\n\n\"Probably not when it is several days old,\" you remark, grimacing at the smell wafting from his goods.\n\nHe fixes you with a stare of outrage. \"Then buy one of my pots and take up fishing yourself!\" he cries, thrusting a lobster pot towards you. \"There! Destroy my livelihood, if you wish! I will sell you this pot for only two cacao.\"\n\nAs you turn to go, he adds: \"I would give anything for a taste of decent bread. I have been on the road for two days with nothing but my own fish to sustain me -- and, as you so tersely put it, they are no longer of the best quality.\"";
        Image = "images/filler1.png";

        Choices.clear();
        Choices.clear();
        Choices.push_back(Choice::Base("Trade a parcel of MAIZE CAKES", 211, Choice::Type::ITEM, Item::Type::MAIZE_CAKES));
        Choices.push_back(Choice::Base("Go north", 120, Choice::Type::NORMAL));
        Choices.push_back(Choice::Base("Go south", 165, Choice::Type::NORMAL));

        Controls = ShopControls();

        Shop = {{Item::Type::LOBSTER_POT, 2}};
    }

    int Continue(Character::Base &player) { return 163; }
};

class Story208 : public Story::Base
{
public:
    std::string PreText = "";

    Story208()
    {
        ID = 208;

        Image = "images/filler1.png";

        Controls = StandardControls();
    }

    void Event(Character::Base &player)
    {
        int selected = 0;

        PreText = "";

        if (Character::VERIFY_ITEM(player, Item::Type::MAGIC_POTION))
        {
            PreText += std::string(Item::Descriptions[Item::Type::MAGIC_POTION]);
            PreText += "\n\nIt can be used once during your adventure. It will restore 5 lost Life Points, up to the limit of your initial Life Points score.\n";

            selected++;
        }

        if (Character::VERIFY_ITEM(player, Item::Type::GREEN_MIRROR))
        {
            if (selected > 0)
            {
                PreText += "\n";
            }

            PreText += std::string(Item::Descriptions[Item::Type::GREEN_MIRROR]);
            PreText += "\n\nIt can be used once -- and only once -- at any point in your adventure before deciding which you will choose.\n";

            selected++;
        }

        if (Character::VERIFY_ITEM(player, Item::Type::JADE_SWORD))
        {
            if (selected > 0)
            {
                PreText += "\n";
            }

            PreText += std::string(Item::Descriptions[Item::Type::JADE_SWORD]);
            PreText += "\n\nIt counts as both a sword and a wand for the purposes of skill-use.\n";

            selected++;
        }

        if (selected > 0)
        {
            Text = PreText.c_str();
        }
        else
        {
            Text = "You chose not to take any of your clan's ancestral treasures.";
        }
    }

    int Continue(Character::Base &player) { return 93; };
};

class Story231 : public Story::Base
{
public:
    Story231()
    {
        ID = 231;
        Text = "The high priest of the War God is a grizzled old soldier, sturdy in spite of his years. You find him at the arena practising the ball contest which is both a sport and a sacred ritual for your people. Clad in heavy protective padding, he swipes at the rubber ball with his forearms and knees, now and again running up along the slanting walls of the arena to drive the ball towards the goal: a stone ring set high up off the arena floor. You watch for a while, marvelling at his strength and grace. Each impact of the ball costs him an effort which can be heard in his grunts and gasps, but he plays on despite the heat of the afternoon, which has sent many a younger man off to a siesta.\n\nAt last he concludes his practice. Pulling off his protective helmet, he wipes back his sweat-soaked greying hair and walks towards you. \"So you're Evening Star,\" he says, clasping your hand. \"Going after your brother, are you? Good, I admire that! Sort that damned sorcerer out, eh?\"\n\nIt is not the custom of your people to be so direct, and is manner leaves you discomposed. \"Um... your ball practice was very impressive,\" you say lamely.\n\n\"For someone of my age, you were going to say?\" He laughs heartily. \"Well, I prefer a bit of killing, if the truth be told, but Koba's not at war with anyone at the moment. Now, as to this quest of yours -- I take it you'll be going by the land route? Take the causeway as far as Yashuna, then turn south and head cross-country of Nachan. There's fine deer to be had in the forest, I can tell you. You are taking all this in, aren't you?\"\n\n\"Er, yes.\"\n\n\"Good. Now, watch out for the stabai when you're in the forest. They're sort of magical nymphs -- can be mischievous, or downright nasty. After Nachan you'll head up through the mountains to the western desert. Make sure you've got a waterskin, by the way, or you won't survive two days in the desert. Do you want to make an offering to the god?\"\n\nAn offering might bring you good fortune on your journey.";
        Image = "images/filler1.png";

        Choices.clear();
        Choices.push_back(Choice::Base("Make a Donation", 301, Choice::Type::GIVE_MONEY));
        Choices.push_back(Choice::Base("Leave the Temple", 301, Choice::Type::NORMAL));

        Controls = StandardControls();
    }

    void Event(Character::Base &player)
    {
        Story::DONATION = 0;
    }
};

class Story234 : public Story::Base
{
public:
    Story234()
    {
        ID = 234;
        Text = "A young man and woman are brought forward by the priests and led to a shrine at the western edge of the hole. A steep flight of steps descends from the shrine towards a platform covered with sacred glyphs. As golden pectorals are placed over the couple's shoulders, it becomes clear that they are going to be sacrificed. They have chosen to jump into the sinkhole, carrying the people's prayers to the Rain God who dwells under the world.";
        Image = "images/filler1.png";

        Choices.clear();

        Controls = StandardControls();
    }

    int Continue(Character::Base &player)
    {
        if (Character::VERIFY_CODEWORD(player, Codeword::Type::CENOTE))
        {
            Character::REMOVE_CODEWORD(player, Codeword::Type::CENOTE);

            return 103;
        }
        else
        {
            return 127;
        }
    }
};

class Story254 : public Story::Base
{
public:
    Story254()
    {
        ID = 254;
        Text = "The Moon Goddess has a small shrine off the northern edge of the temple plaza. You look up the pyramid steps to where the high priest awaits you, and he beckons for you to ascend. You compress your lips in annoyance; you had hoped he would come down to you. Even a small temple involves a steep climb.\n\nAs you make your way up the steps, you see stone effigies that depict the Nine Lords of the Night bearing the full moon up towards the shrine at the summit of the pyramid. The effigies are painted in the simple cream-gold hue of moonlight, with none of the bright daubings favoured by the other temples.\n\nThe high priest, too, has a manner quite unlike the priests of other gods. He wears a plain white robe, and a thin silver chain hangs around his waist. His smile of welcome seems modest and unaffected, but you sense a slight air of smugness behind the diffidently averted eyes. \"Good afternoon,\" he says. \"You must be Evening Star.\"\n\n\"Let me get my breath back,\" you say, stooping as you reach the top of the steps. The baking sun on your back sends rivulets of sweat trickling off your brow. You glance down at the plaza twenty metres below. \"That's quite a climb.\"\n\nThe high priest smiles. \"You are out of condition.\"\n\nYou give him a wry look and sweep out your arm to indicate the flat landscape of fields and savannah surrounding the city. \"In these parts, only the holy get plenty of climbing practice. I've come to you for advice on my quest into the western desert.\"\n\n\"Buy a waterskin.\"\n\nYou're unsure how to take that remark. You watch him, but the only trace that he might be joking is a sly curl of the lips. \"Is that all you have to suggest?\" you ask.\n\nHe glances at the shrine behind him. \"See this stucco? Flaking away, I'm afraid. The whole outer facade needs repairs.\"\n\nIn his roundabout way, he's asking for a donation.";
        Image = "images/filler1.png";

        Choices.clear();
        Choices.push_back(Choice::Base("Make a DONATION (1 cacao)", 424, Choice::Type::LOSE_MONEY, 1));
        Choices.push_back(Choice::Base("Make a DONATION (2 cacao)", 2, Choice::Type::LOSE_MONEY, 2));
        Choices.push_back(Choice::Base("You are not prepared to make any donation and you need to hurry over to the market and spend your money on supplies instead", 93, Choice::Type::NORMAL));

        Controls = StandardControls();
    }
};

class Story257 : public Story::Base
{
public:
    Story257()
    {
        ID = 257;
        Text = "The priest sees your bracelet and turns to look at you with new interest. You do not entire like the expression of alert scrutiny on his face. He reminds you of an eagle studying a mouse. \"Ah, I see you are one of the chosen,\" he says, calling to a group of priestly warriors near by.\n\n\"The chosen what?\" you ask.\n\nHe gives you a puzzled look. \"Why, one of those chosen to carry our petition to the Rain God,\" he replies.\n\nThe guards close in at your shoulders. The priest gestures towards the sunken lake, and suddenly the truth dawns. They mean to cast you into the pit as a living sacrifice to the gods.";
        Image = "images/filler1.png";

        Choices.clear();
        Choices.push_back(Choice::Base("Resist the fate they have in store for you", 281, Choice::Type::NORMAL));
        Choices.push_back(Choice::Base("Cast a protective enchantment (SPELLS)", 304, Choice::Type::SKILL_ANY, Skill::Type::SPELLS, {Item::Type::MAGIC_WAND, Item::Type::JADE_SWORD}));
        Choices.push_back(Choice::Base("Agree to being thrown into the pit", 327, Choice::Type::NORMAL));

        Controls = StandardControls();
    }
};

class Story277 : public Story::Base
{
public:
    Story277()
    {
        ID = 277;
        Text = "The high priest of the Death God is a portly short-sighted man wrapped in a black kilt. White paint is daubed in streaks around his belly as if to represent the fleshless ribcage of a corpse -- though you cannot help smiling as you reflect how his own ribs are so well covered. His head-dress is a rather intimidating effigy of a skull without its lower jaw, with long earpieces of jointed fingerbones hanging on neither side of his rotund face, but the priest removes this as soon as you are both seated in the shade of the outer shrine.\n\n\"I have come to you for advice,\" you begin by saying, \"I must undertake a journey to the west in search of my brother, Morning Star\"\n\nHe wipes a slick of sweat off his brow with one plump hand. \"I have heard the story. Only one man has entered the great desert and left it alive -- the veteran who accompanied your brother. What makes you think you will fare better?\"\n\n\"One man survived. Why shouldn't I\"\n\nThe priest shakes his head. \"He was one man from an expedition of thirty. Did you not hear his account of the perils of the desert -- the devil-driven sands, the monstrous serpents?\"\n\n\"Then you counsel me to stay here in Koba? To abandon my quest?\"\n\nHe casts a quick gaze towards the shadowy archway leading to the inner shrine. \"The advice of the god is not so easily obtained. Are you willing to make an offering?\"";
        Image = "images/filler1.png";

        Choices.clear();
        Choices.push_back(Choice::Base("Make an OFFERING (5 cacao)", 408, Choice::Type::LOSE_MONEY, 5));
        Choices.push_back(Choice::Base("You cannot spare the money and must bid the priest farewell and see about getting supplies for the trip", 93, Choice::Type::NORMAL));

        Controls = StandardControls();
    }
};

class Story301 : public Story::Base
{
public:
    Story301()
    {
        ID = 301;

        Choices.clear();
        Controls.clear();
    }

    int Background(Character::Base &player)
    {
        if (Story::DONATION > 4)
        {
            return 323;
        }
        else
        {
            return 346;
        }
    }
};

class Story323 : public Story::Base
{
public:
    Story323()
    {
        ID = 323;
        Text = "\"A generous donation.\" The high priest puts the money into his belt-pouch. \"I shall see that a ritual of benediction is performed for you at the temple tomorrow morning. You will carry the blessing of the god.\n\nThe blessing of the War God means that you can ignore any one injury taken at any point of your choice during the adventure. Remember that it only works once.\n\n\"How about a spot of ball practice?\" suggests the high priest.";
        Image = "images/filler1.png";

        Choices.clear();
        Choices.push_back(Choice::Base("Join him for a practice bout in the arena", 368, Choice::Type::NORMAL));
        Choices.push_back(Choice::Base("You had better get on with the rest of your preparations for the journey", 93, Choice::Type::NORMAL));

        Controls = StandardControls();
    }

    void Event(Character::Base &player)
    {
        Story::BLESSING_WAR_GOD = true;
    }
};

class Story346 : public Story::Base
{
public:
    Story346()
    {
        ID = 346;
        Text = "The high priest looks disappointed, but seems to understand when you explain that you have barely enough to purchase the supplies you will need for the journey.\n\n\"A coil of rope might not be a bad idea,\" he suggests. \"I've often found the rope to come in handy. Oh, and a decent sword of course -- assuming you know how to use one.\" He juggles the heavy rubber ball he's carrying from one hand to the other. \"What about a practice session in the arena before you head off?\"";
        Image = "images/filler1.png";

        Choices.clear();
        Choices.push_back(Choice::Base("Practice the ball contest with him", 368, Choice::Type::NORMAL));
        Choices.push_back(Choice::Base("Get on with preparations for your journey", 93, Choice::Type::NORMAL));

        Controls = StandardControls();
    }
};

class Story350 : public Story::Base
{
public:
    Story350()
    {
        ID = 350;
        Text = "You get your first glimpse of Yashuna on the afternoon of the following day. Like Koba, the tall temple pyramids of the city are visible for miles across the rolling countryside. As you approach the poorer dwellings at the outskirts of the city, you notice a large number of people milling towards the raised central complex which houses the temples and the palaces of the rich.\n\nIt seems to be market day.";
        Image = "images/yashuna.png";

        Choices.clear();
        Choices.push_back(Choice::Base("Pay a visit to the market", 142, Choice::Type::NORMAL));
        Choices.push_back(Choice::Base("Head north out of the city", 120, Choice::Type::NORMAL));
        Choices.push_back(Choice::Base("Take the southern road towards the forest", 165, Choice::Type::NORMAL));

        Controls = StandardControls();
    }
};

class Story368 : public Story::Base
{
public:
    Story368()
    {
        ID = 368;
        Text = "He flings the the ball against the slanting wall of the arena and it bounces towards you.. Leaping up with all your strength, you manage to strike it a glancing blow with your arm. A moment of stinging pain is followed by numbness, and the ball thuds to the court at your feet.\n\n\"No, no!\" says the priest, stepping forward to retrieve the ball. \"You have to judge the angle of impact perfectly, otherwise you end up bruised and the ball goes nowhere.\"\n\nHe demonstrates, swinging his hips for momentum a she slams his muscular forearm against the ball. It goes careering across the arena to ricochet off the side wall. This time you manage to intercept it, sending it skidding back along the slant of the wall towards the priest.\n\n\"You've got the hang of it now,\" he says some time later, leaning on his knees to get his breath back. \"The object is to keep the ball from touching the ground. It represents the sun, you see, so it has to stay aloft or else it's 'eclipse', meaning you lose a point.\"\n\nYou toss the ball up and catch it, enjoying the contest now you know more about it. \"And what about the stone rings at the top of the wall?\"\n\n\"Ah!\" says the priest. \"If you can get the ball through the ring then you've scored a 'sunrise' and you win outright. But it's not easy as it looks, believe me.\"\n\nYou stare up at the ring. It is tree man-heights off the ground and barely wider than the ball itself. \"But it doesn't look at all easy!\"\n\nThe priest grins. \"Precisely.\"\n\nIt is now getting late. Thanking the priest, you make your way back home. Tomorrow you will set out in search of your brother.\n\nYou gained the codeword POKTAPOK.";
        Image = "images/filler1.png";

        Choices.clear();

        Controls = StandardControls();
    }

    int Continue(Character::Base &player) { return 389; }

    void Event(Character::Base &player)
    {
        Character::GET_CODEWORDS(player, {Codeword::Type::POKTAPOK});
    }
};

class Story389 : public Story::Base
{
public:
    Story389()
    {
        ID = 389;
        Text = "You decide to set out early the next morning, before sunrise. This will spare your family from farewells. By lantern light in the chill grey predawn, you stand in the antechamber of your family's house and check your belongings for the journey. You are attended by only one servant, who silently fastens the straps of your pack. Your aunts have left out a parcel of MAIZE CAKES for you to eat on the road.\n\nThere is a knock on the outer door and the servant darts off to open it. Outside you see your friend the old soothsayer standing in the early twilight. You go out and greet him: \"Good morning. You came just in time to catch me. I'm about to set out.\"\n\n\"I know,\" he says. \"I came to wish you luck. And to give you this.\" He holds up a JADE BEAD.\n\nYou take it with a quizzical smile. \"What's this for?\"\n\n\"There are some who'll tell you that the quickest route to Necklace of Skulls lies through the underworld. It is true, but that way is also fraught with peril and you will need certain safeguards if you hope to pass through in safety. Now, beads such as this are placed under the tongue of deceased nobles for them to use as currency in the afterlife. If you should enter the underworld, be sure to place the bead under your tongue and to keep it there until you reach the crossroads. Got that?\"\n\n\"I suppose so,\" you say, not really following his drift at all. But you pocket the JADE BEAD. As you set out along the road, you pause and glance back, adding, \"You were wrong about the dream. It seems my brother was dead after all.\"\n\nHe shrugs. \"Right... wrong... The world isn't quite that simple, Evening Star.\"\n\nBidding him farewell, you set off towards the edge of the city. Even at this early hour, traders are already carrying their wares to market. Out in the fields, moving shadows in the smoky blue twilight show that the farmers are hard at work. It is strange to think that you might never again see this great city of Koba, which has been your home since childhood.\n\nYou turn your gaze to the west, putting such thoughts out of your head. From now on, you must think only of the success of your quest.";
        Image = "images/filler1.png";

        Choices.clear();
        Controls = StandardControls();
    }

    int Continue(Character::Base &player) { return 25; }

    void Event(Character::Base &player)
    {
        Character::GET_ITEMS(player, {Item::Type::JADE_BEAD, Item::Type::MAIZE_CAKES});
    }
};

class Story408 : public Story::Base
{
public:
    Story408()
    {
        ID = 408;
        Text = "The high priest winds a white cloth across your eyes and leads you through to the inner shrine. A deep chill abides here; the thick stone blocks of the Death God's temple walls are never warmed by the sun. The sweet tarry smell of incense hangs in the air. You feel a hand on your shoulder, guiding you to kneel.\n\nA long period of utter silence ensues. You did not hear the high priest withdraw from the chamber, but you gradually become sure that he has left you here alone. You dare not remove the blindfold; to gaze directly on the holy of holies would drive you instantly insane.\n\nA whispering slithers slowly out of the silence. At first you take it for a trick of your unsettle imagination, but by straining your ears you begin to make out words. \"The way to the west lies through the underworld,\" the whispering tells you. \"Go to the city of Yashuna. North of the city lies a sacred well which is the entrance to the underworld. Take this path, which is dangerous but swift, and you will emerge at the western rim of the world. From there it is but a short journey back through the desert to your goal.\"\n\nThe whispers fade, drowned out by the thudding of your heart. Frozen with terror at the words of the god, yo crouch motionless on the cold flagstones. The cloying scent of incense grows almost unbearable.\n\nSuddenly a hand touches your shoulder. After the initial jolt of alarm, you allow yourself to be led out onto the portico of the temple, where the blindfold is removed. You blink in the dazzling sunlight. You feel as weak as a baby and the smell of incense clings to your clothes. After the cool of the shrine, the heat of the afternoon sun makes you feel slightly sick.\n\nThe podgy priest is looking up into your eyes. \"You head the voice of the god,\" he says simply.\n\nYou gained the codeword CENOTE.";
        Image = "images/filler1.png";

        Choices.clear();

        Controls = StandardControls();
    }

    int Continue(Character::Base &player) { return 93; }

    void Event(Character::Base &player)
    {
        Character::GET_CODEWORDS(player, {Codeword::Type::CENOTE});
    }
};

class Story424 : public Story::Base
{
public:
    Story424()
    {
        ID = 424;
        Text = "\"The goddess oversees the tides,\" says the priest, accepting your gift with a heavy-lidded smile. \"Betake yourself to the town of Balak. Do not put to sea at once; wait a week before you set sail. Stay close to shore if you wish to avoid a hazardous adventure.\"\n\nYou wait for more, but he seems to have finished speaking. \"You haven't even mentioned the western desert,\" you say in a tone verging on indignation.\n\n\"I have given my advice on that. There is just one think I might add: seek the blood that is like sap.\"\n\nYou suck your teeth. \"Enigmatic,\" you comment drily.\n\n\"If it's clarity you're after, consult a merchant.\"\n\nSomewhat disgruntled, you retreat down the temple steps. You would have preferred to proudly turn your back on the smug priest, but custom forbids you to avert your face from any shrine as you leave it for fear of offending the deity within. Also, these temple steps are alarmingly steep; descending backwards is less nerve-wracking.";
        Image = "images/filler1.png";

        Choices.clear();

        Controls = StandardControls();
    }

    int Continue(Character::Base &player) { return 93; };
};

class NotImplemented : public Story::Base
{
public:
    NotImplemented()
    {
        ID = -1;
        Title = "Not implemented yet";

        Controls.clear();
        Controls.push_back(Button(0, "images/exit.png", 0, 0, -1, -1, (1 - Margin) * SCREEN_WIDTH - buttonw, buttony, Control::Type::BACK));
    }
};

auto notImplemented = NotImplemented();

auto Stories = std::vector<Story::Base *>();

void *findStory(int id)
{
    Story::Base *story = &notImplemented;

    if (Stories.size() > 0)
    {
        for (auto i = 0; i < Stories.size(); i++)
        {
            if (((Story::Base *)Stories[i])->ID == id)
            {
                story = (Story::Base *)Stories[i];

                break;
            }
        }
    }

    return story;
}

auto prologue = Prologue();
auto story001 = Story001();
auto story002 = Story002();
auto story024 = Story024();
auto story025 = Story025();
auto story047 = Story047();
auto story048 = Story048();
auto story070 = Story070();
auto story071 = Story071();
auto story093 = Story093();
auto story094 = Story094();
auto story096 = Story096();
auto story103 = Story103();
auto story116 = Story116();
auto story117 = Story117();
auto story120 = Story120();
auto story138 = Story138();
auto story139 = Story139();
auto story142 = Story142();
auto story162 = Story162();
auto story163 = Story163();
auto story165 = Story165();
auto story185 = Story185();
auto story186 = Story186();
auto story188 = Story188();
auto story208 = Story208();
auto story231 = Story231();
auto story234 = Story234();
auto story254 = Story254();
auto story257 = Story257();
auto story277 = Story277();
auto story301 = Story301();
auto story323 = Story323();
auto story346 = Story346();
auto story350 = Story350();
auto story368 = Story368();
auto story389 = Story389();
auto story408 = Story408();
auto story424 = Story424();

void InitializeStories()
{
    Stories = {
        &prologue, &story001, &story002, &story024, &story025,
        &story047, &story048, &story070, &story071,
        &story093, &story094, &story096, &story103, &story116, &story117, &story120,
        &story138, &story139, &story142, &story162, &story163, &story165, &story185, &story188,
        &story186, &story208, &story231, &story234, &story254, &story257, &story277, &story301, &story323, &story346,
        &story350, &story368, &story389, &story408, &story424};
}

#endif
