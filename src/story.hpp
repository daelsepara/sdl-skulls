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
        ALL_ITEMS,
        ANY_SKILL,
        SKILL_ITEM,
        SKILL_ANY,
        SKILL_ALL,
        FIRE_WEAPON,
        LOSE_ITEM,
        LOSE_MONEY,
        LOSE_ALL,
        LOSE_SKILLS,
        DONATE,
        EAT,
        EAT_HEAL,
        GIFT
    };

    class Base
    {
    public:
        const char *Text = NULL;

        Choice::Type Type = Choice::Type::NORMAL;

        Skill::Type Skill = Skill::Type::NONE;

        std::vector<Item::Type> Items = std::vector<Item::Type>();
        std::vector<std::pair<Item::Type, int>> Gifts = std::vector<std::pair<Item::Type, int>>();

        Item::Type Item = Item::Type::NONE;

        Codeword::Type Codeword = Codeword::Type::NONE;

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

        Base(const char *text, int destination)
        {
            Text = text;
            Destination = destination;
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

        Base(const char *text, int destination, Codeword::Type codeword)
        {
            Text = text;
            Destination = destination;
            Type = Choice::Type::CODEWORD;
            Codeword = codeword;
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

        Base(const char *text, int destination, Choice::Type type, std::vector<Item::Type> items, int value)
        {
            Text = text;
            Destination = destination;
            Type = type;
            Items = items;
            Value = value;
        }

        Base(const char *text, int destination, std::vector<std::pair<Item::Type, int>> gifts)
        {
            Text = text;
            Destination = destination;
            Type = Choice::Type::GIFT;
            Gifts = gifts;
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
        DOOM
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
        std::pair<Item::Type, Item::Type> Trade;

        // Player selects items to take up to a certain limit
        std::vector<Item::Type> Take = std::vector<Item::Type>();

        // Player selects items to lose
        std::vector<Item::Type> ToLose = std::vector<Item::Type>();

        int Limit = 0;

        int LimitSkills = 0;

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
} // namespace Story

std::vector<Button> StandardControls()
{
    auto controls = std::vector<Button>();

    controls.push_back(Button(0, "icons/up-arrow.png", 0, 1, 0, 1, (1.0 - Margin) * SCREEN_WIDTH - arrow_size, texty + border_space, Control::Type::SCROLL_UP));
    controls.push_back(Button(1, "icons/down-arrow.png", 0, 2, 0, 2, (1.0 - Margin) * SCREEN_WIDTH - arrow_size, texty + text_bounds - arrow_size - border_space, Control::Type::SCROLL_DOWN));
    controls.push_back(Button(2, "icons/map.png", 1, 3, 1, 2, startx, buttony, Control::Type::MAP));
    controls.push_back(Button(3, "icons/disk.png", 2, 4, 1, 3, startx + gridsize, buttony, Control::Type::GAME));
    controls.push_back(Button(4, "icons/user.png", 3, 5, 1, 4, startx + 2 * gridsize, buttony, Control::Type::CHARACTER));
    controls.push_back(Button(5, "icons/items.png", 4, 6, 1, 5, startx + 3 * gridsize, buttony, Control::Type::USE));
    controls.push_back(Button(6, "icons/next.png", 5, 7, 1, 6, startx + 4 * gridsize, buttony, Control::Type::NEXT));
    controls.push_back(Button(7, "icons/exit.png", 6, 7, 1, 7, (1.0 - Margin) * SCREEN_WIDTH - buttonw, buttony, Control::Type::BACK));

    return controls;
}

std::vector<Button> ShopControls()
{
    auto controls = std::vector<Button>();

    controls.push_back(Button(0, "icons/up-arrow.png", 0, 1, 0, 1, (1.0 - Margin) * SCREEN_WIDTH - arrow_size, texty + border_space, Control::Type::SCROLL_UP));
    controls.push_back(Button(1, "icons/down-arrow.png", 0, 2, 0, 2, (1.0 - Margin) * SCREEN_WIDTH - arrow_size, texty + text_bounds - arrow_size - border_space, Control::Type::SCROLL_DOWN));
    controls.push_back(Button(2, "icons/map.png", 1, 3, 1, 2, startx, buttony, Control::Type::MAP));
    controls.push_back(Button(3, "icons/disk.png", 2, 4, 1, 3, startx + gridsize, buttony, Control::Type::GAME));
    controls.push_back(Button(4, "icons/user.png", 3, 5, 1, 4, startx + 2 * gridsize, buttony, Control::Type::CHARACTER));
    controls.push_back(Button(5, "icons/items.png", 4, 6, 1, 5, startx + 3 * gridsize, buttony, Control::Type::USE));
    controls.push_back(Button(6, "icons/shop.png", 5, 7, 1, 6, startx + 4 * gridsize, buttony, Control::Type::SHOP));
    controls.push_back(Button(7, "icons/next.png", 6, 8, 1, 7, startx + 5 * gridsize, buttony, Control::Type::NEXT));
    controls.push_back(Button(8, "icons/exit.png", 7, 8, 1, 8, (1.0 - Margin) * SCREEN_WIDTH - buttonw, buttony, Control::Type::BACK));

    return controls;
}

std::vector<Button> TradeControls()
{
    auto controls = std::vector<Button>();

    controls.push_back(Button(0, "icons/up-arrow.png", 0, 1, 0, 1, (1.0 - Margin) * SCREEN_WIDTH - arrow_size, texty + border_space, Control::Type::SCROLL_UP));
    controls.push_back(Button(1, "icons/down-arrow.png", 0, 2, 0, 2, (1.0 - Margin) * SCREEN_WIDTH - arrow_size, texty + text_bounds - arrow_size - border_space, Control::Type::SCROLL_DOWN));
    controls.push_back(Button(2, "icons/map.png", 1, 3, 1, 2, startx, buttony, Control::Type::MAP));
    controls.push_back(Button(3, "icons/disk.png", 2, 4, 1, 3, startx + gridsize, buttony, Control::Type::GAME));
    controls.push_back(Button(4, "icons/user.png", 3, 5, 1, 4, startx + 2 * gridsize, buttony, Control::Type::CHARACTER));
    controls.push_back(Button(5, "icons/items.png", 4, 6, 1, 5, startx + 3 * gridsize, buttony, Control::Type::USE));
    controls.push_back(Button(6, "icons/shop.png", 5, 7, 1, 6, startx + 4 * gridsize, buttony, Control::Type::TRADE));
    controls.push_back(Button(7, "icons/next.png", 6, 8, 1, 7, startx + 5 * gridsize, buttony, Control::Type::NEXT));
    controls.push_back(Button(8, "icons/exit.png", 7, 8, 1, 8, (1.0 - Margin) * SCREEN_WIDTH - buttonw, buttony, Control::Type::BACK));

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
        Choices.push_back(Choice::Base("Reply that the life of your brother is more important than your duty to the clan", 24));
        Choices.push_back(Choice::Base("...that on the contrary, clan honour demands that you go", 47));
        Choices.push_back(Choice::Base("Say nothing", 70));

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

        Shop = {{Item::Type::OWL, 2}};

        Controls = ShopControls();
    }

    int Continue(Character::Base &player) { return 93; }
};

class Story003 : public Story::Base
{
public:
    Story003()
    {
        ID = 3;

        Text = "You are suddenly embroiled in a furious fight while clinging precariously ten metres off the ground. The tree shakes with the stirrings of the mighty beast as it twists its taloned hand to and fro, rumbling its rage in a voice like a hurricane.\n\nYou have barely seconds to decide your tactics.";

        Image = "images/filler1.png";

        Choices.clear();
        Choices.push_back(Choice::Base("Grab the arm and let it pull you inside the bole of the dead tree", 141));
        Choices.push_back(Choice::Base("Chop at it as it flails blindly for you", 164));

        Controls = StandardControls();
    }
};

class Story004 : public Story::Base
{
public:
    Story004()
    {
        ID = 4;

        Text = "You are not sure which nuts and berries are safe to eat, but extreme hunger forces you to make do. You wash the berries down with water from a brook. The meal is meagre and unsatisfying, but at least it eases the pangs in your stomach. Even so, you realize that you cannot survive for long if you don't find your way back to civilization.\n\nYou LOSE 1 Life Point.";

        Image = "images/filler1.png";

        Choices.clear();

        Controls = StandardControls();
    }

    void Event(Character::Base &player)
    {
        Character::GAIN_LIFE(player, -1);
    }

    int Continue(Character::Base &player) { return 279; }
};

class Story005 : public Story::Base
{
public:
    Story005()
    {
        ID = 5;

        Text = "You steel your nerves and leap over the edge. The water rushes up to meet you, enfolding you in a silent icy embrace. The shock of impact drives the air out of your lungs and you start to fail wildly as you go under. The weight of your gold regalia drags you down, and as you fumble with the straps it becomes obvious that you will run out of air long before you can get free.\n\nThen you remember your BLOWGUN. Thrusting up through the water with it, you pierce the glimmering pane of light that marks the surface and blow into the other end until you have forced water out and can draw down a mouthful of fresh air. Using the BLOWGUN as a breathing tube buys you the time you need to struggle out of the encumbering regalia and swim up to safety.\n\nThe moment your head breaks the surface you know you are no longer at the bottom of the sacred well. Instead of the open sky overhead, there is just the roof of a large cavern. Grey light trickles from an unseen source.\n\nA familiar sound echoes off the surrounding rocks. You turn to see a canoe being slowly paddled towards you. But the two oarsmen are like no others on earth...";

        Image = "images/filler1.png";

        Choices.clear();

        Controls = StandardControls();
    }

    int Continue(Character::Base &player) { return 97; }
};

class Story006 : public Story::Base
{
public:
    Story006()
    {
        ID = 6;

        Text = "The forest, so lushly appealing when you first entered it, now seems a hellish green labyrinth. The tangled paths are indistinguishable. You might be going in circles.";

        Image = "images/filler1.png";

        Choices.clear();
        Choices.push_back(Choice::Base("Go right", 75));
        Choices.push_back(Choice::Base("Go left", 160));
        Choices.push_back(Choice::Base("Go straight ahead", 98));

        Controls = StandardControls();
    }
};

class Story007 : public Story::Base
{
public:
    Story007()
    {
        ID = 7;

        Text = "You hear a sound. It is quiet and unsettling. It sounded like a rattling intake of breath in a dry dead throat. You have the sudden impression that something has become aware of your presence here. Something is watching you.\n\nYou look back. The tunnel behind the boat is now filled with ochre mist that hangs in the air in long sparkling veils. As you watch, you see the cloud getting thicker as more mist seeps out of the edges of the rock tombs.\n\nThe demon in the back of the canoe also takes a glance back. He pauses stock-still for a moment, then explodes into frantic action. Paddling furiously, he yells to the other demon, \"They've woken up! Let's get out of here!\"\n\nToo late. The ochre cloud suddenly comes rushing forward as though blown by a blast of wind. The boat is completely enveloped. The demons are obviously expecting trouble and do not wait around. \"Abandon ship!\" you hear the one in the prow yelling, and then two heavy splashes can be heard as they jump overboard.\n\nPhantom figures loom out of the mist, pressing their skeletal faces close to yours and clutching at you with thin fingers of yellow bone. You are rigid with terror; you cannot even find the breath to scream. The mist sends a chill deep into you that no fire will ever be able to warm again.\n\nYour maximum Life Points is PERMANENTLY REDUCED by 2.\n\nThe mist retreats as rapidly as it appeared, drawing back in long wisps into the rock tombs like smoke being sucked back into a row of pipes. But the danger is not over. Without the two oarsmen, your canoe is being carried out of control by the river current.";

        Image = "images/filler1.png";

        Choices.clear();
        Choices.push_back(Choice::Base(Skill::SEAFARING.Name, 76, Skill::Type::SEAFARING));
        Choices.push_back(Choice::Base("Use a ROPE", 99, Item::Type::ROPE));
        Choices.push_back(Choice::Base("Otherwise", 214));

        Controls = StandardControls();
    }

    void Event(Character::Base &player)
    {
        player.MAX_LIFE_LIMIT -= 2;

        Character::GAIN_LIFE(player, 0);
    }
};

class Story008 : public Story::Base
{
public:
    Story008()
    {
        ID = 8;

        Text = "As you go west, the land rises and becomes drier. You leave behind the lush forest, trekking first through windswept moorland and then dusty gulches lined with sparse bracken.\n\nAshaka is a hilltop citadel with palaces set on high terraces cut into the mountainside. It stares down across the scattered farming communities it rules, like an eagle glowering atop a cactus. As you start up the red-paved road that wends up to the citadel, you pass a small man who is bent and toothless with age. \"Going up to Ashaka, are you?\" he cackles. \"They'll be pleased!\"\n\n\"Why is that?\" you ask.\n\nHe puts a finger to one nostril and snorts a gobbet of mucus onto the ground. \"They're after sacrifices,\" he says. \"Priests reckon the gods are annoyed. Must be bloody furious, if you ask me!\"\n\n\"Eh?\" You are puzzled.\n\nHe fixes you with a canny stare. \"The Great City -- it's been sacked by dog-men from the western desert. It's going to take heap of sacrifices to get the gods to rebuild it, hah!\"\n\nWhat he says is inconvenient if true, since you were relying on getting provisions in Ashaka.";

        Image = "images/filler1.png";

        Choices.clear();
        Choices.push_back(Choice::Base("Go up anyway", 275));
        Choices.push_back(Choice::Base("Decide against visiting Ashaka by continuing to travel overland", 298));
        Choices.push_back(Choice::Base("Detour to the sea and travel up the coast to Tahil", 228));

        Controls = StandardControls();
    }
};

class Story009 : public Story::Base
{
public:
    std::string PreText = "";

    Story009()
    {
        ID = 9;

        Image = "images/filler1.png";

        Choices.clear();

        Controls = StandardControls();
    }

    void Event(Character::Base &player)
    {
        PreText = "You descend into the pyramid. The staircase is narrow, steep and dank. Lighting-strokes cast a guttering white glare from above, plunging you into darkness as they pass. The thundercracks in the sky resound ominously through the heavy stone blocks of the pyramid. The steps are slippery with damp, forcing you to make the descent slowly. At last you reach the bottom and pass through a doorway draped with thick fleshy roots. A tunnel stretches ahead which you have to feel your way along. No light penetrates this far down. The smell in the air is of damp soil and limestone.\n\nThe walls vibrate as another thunderbolt shakes the earth. Suddenly you are knocked off your feet by a heavy weight of rubble dropping on you. You realize the tunnel has caved in. Claustrophobia seizes you. Struggling in panic, you claw at the rubble in a frantic attempt to dig yourself free.\n\nYour hands break through to the air and you push up, gasping for breath. You are no longer in the underground tunnel, though. You have emerged into an unearthly landscape. A barren plain stretches away in all directions under a sky of red-tinged darkness. In the distance you can see a haze of sulphurous clouds lit by fiery light: the lip of volcanic fissure. You head towards it.";

        if (Character::VERIFY_ITEMS(player, {Item::Type::JADE_BEAD}))
        {
            PreText += "\n\nYou lodged the JADE BEAD under your tongue as the spirit advised you.";
        }

        PreText += "\n\nYou gained the codeword PAKAL.";

        Character::GET_CODEWORDS(player, {Codeword::Type::PAKAL});

        Text = PreText.c_str();
    }

    int Continue(Character::Base &player) { return 80; }
};

class Story010 : public Story::Base
{
public:
    Story010()
    {
        ID = 10;

        Text = "He pretends to listen with interest to your confident reply, but then gives a great whoop of mocking laughter. You realize he lied -- he knows nothing that can help you in your quest.";

        Image = "images/filler1.png";

        Choices.clear();

        Controls = StandardControls();
    }

    int Continue(Character::Base &player)
    {
        if (Character::VERIFY_ITEMS(player, {Item::Type::JADE_BEAD}))
        {
            return 56;
        }
        else
        {
            return 60;
        }
    }
};

class Story011 : public Story::Base
{
public:
    Story011()
    {
        ID = 11;

        Text = "As the sun sets, the pitcher on the woman's shoulder tilts, falling to reveal that our guess was right -- there is a second head protruding from her neck!\n\nThe nightcrawler's eyes snap open and fix on you, and its mouth drops open in a gurgling snarl. Long strands of black hair shoot out from it like tentacles, some of them up to two metres long. These form into thin matted stalks resembling an insect's legs, which probe the ground, preparing to support the creature's weight- There is a grisly sucking sound as the nightcrawler pulls itself free of the sleeping woman's neck.\n\nIt comes scuttling forward eagerly on its limbs of twined hair and leaps up towards your neck, intending to make you its new host, but you are ready for it. It blunders straight into the net which your magic has woven out of moonbeams and river mist. It struggles and gnashes its long teeth, but it cannot break free of the net. Taking it to the river bank, you cast it into the water. \"And good riddance,\" you say as it sinks to a final resting place on the river bed.";

        Image = "images/filler1.png";

        Choices.clear();

        Controls = StandardControls();
    }

    int Continue(Character::Base &player) { return 398; }
};

class Story012 : public Story::Base
{
public:
    Story012()
    {
        ID = 12;

        Text = "Cleverness is not so much a question of intelligence. It has a lot to do with just having a smart attitude. Where others look for the obvious answer to any problem, you have a habit of being contrary -- of always trying the unexpected first. You find it usually works.\n\nLike now: the average person would climb to the top of the steps and probably have to tackle some devious puzzle or demonic monster in the shrine above. You check out the opposite approach, and discover that the steps not only lead up but also descend beneath the water. It's a near certainty that this is the route you must take.";

        Image = "images/filler1.png";

        Choices.clear();

        Controls = StandardControls();
    }

    int Continue(Character::Base &player) { return 105; }
};

class Story013 : public Story::Base
{
public:
    std::string PreText = "";

    Story013()
    {
        ID = 13;

        Image = "images/filler1.png";

        Choices.clear();

        Controls = StandardControls();
    }

    void Event(Character::Base &player)
    {
        PreText = "A peasant shows up at last, emerging on the adjacent ledge beside the river. Time passes in a dream-like way here; you cannot tell if hours or days have gone by while you were waiting. You keenly feel the urgency of your quest.\n\nThe noble gets to his feet. \"Since I was here first, I shall travel on with this man,\" he says to you once he has explained the situation to the peasant. \"No doubt another poor man will arrive eventually. I'll throw back the poles once I'm across, so that you will be able to use them.\"\n\nDespite the fact that he was here first, you have to insist that you go across at once. He reacts with indignation, and a struggle ensues. A life of luxury has left him no match for you, with your ardency and youthful vigour. You subdue him and take the pole for yourself.";

        if (!Character::VERIFY_SKILL_ANY(player, Skill::Type::SWORDPLAY, {Item::Type::SWORD, Item::Type::JADE_BEAD}) && !Character::VERIFY_SKILL(player, Skill::Type::UNARMED_COMBAT))
        {
            Character::GAIN_LIFE(player, -1);

            PreText += "\n\nYou LOSE 1 Life Point.";
        }

        if (player.Life > 0)
        {
            PreText += "\n\nHooking your pole with the peasant's, the two of you are able to cross the obsidian beams by setting your feet against the flat sloping sides of the beams and using your weight to counterbalance each other. When you reach the far side, you throw the poles back to the noble, but he is so enraged that he allows them to fall into the river. \"May all the gods curse you!\" he cries, shaking his fist. \"You are without honour!\"\n\nYou shrug and turn away. Though you regret what you had to do, it was necessary if you are to save your brother.";
        }

        Text = PreText.c_str();
    }

    int Continue(Character::Base &player) { return 36; }
};

class Story014 : public Story::Base
{
public:
    Story014()
    {
        ID = 14;

        Text = "You address the sentinel by what you think is his name, only to realize your mistake at once. He gives a high howl of immortal outrage and lashes out with his bone sceptre. It slices through the air like a falling star, giving you no chance to react. You hear yourself cry out in agony at the blow, but the curious thing is that you never discover where you have been hurt. By the time you look down, you are already dead.";

        Image = "images/filler1.png";

        Type = Story::Type::DOOM;

        Choices.clear();

        Controls = StandardControls();
    }
};

class Story015 : public Story::Base
{
public:
    Story015()
    {
        ID = 15;

        Text = "The warrior stands looking at the hydra's body for a few moments in frank astonishment, then turns and nods to you. \"Well done,\" he says. \"Few men could have equalled that feat.\"\n\n\"My name is Evening Star,\" you tell him. \"I am from the city of Koba, and I have travelled the breadth of the world in search of my brother.\"\n\n\"I am Stooping Eagle, until recently a lord of the Great City. I had been on a voyage, and returned to discover my home had been ransacked by werewolves and night demons. All my people are either slain or fled, and the Great City is now a sad empty ruin. Thus I have come to wreak vengeance on the one responsible: a sorcerer known as Necklace of Skulls.\"\n\n\"Then we are partners in the same quest, for I too desire the sorcerer's death.\"\n\n\"I had heard he was already dead, but none the less active for that,\" says Stooping Eagle. He turns and surveys the moonlit dunes. \"But the desert is wide, and I have been searching for his palace since the morning star last rose.\"\n\nYou smile at the unconscious aptness of his remark. \"It is close,\" you assure him. \"I feel sure of it.\"\n\nYou walk through the night, sheltering by day in the shade of a crag before heading on a sunset. After several hours Stooping Eagle unstoppers his waterskin and rations out a few sips. It is barely enough to moisten his lips, but he looks like a man who is used to austerity.";

        Image = "images/filler1.png";

        Choices.clear();

        Controls = StandardControls();
    }

    int Continue(Character::Base &player)
    {
        if (Character::VERIFY_ITEMS(player, {Item::Type::WATERSKIN}))
        {
            return 220;
        }
        else if (Character::VERIFY_SKILL(player, Skill::Type::ETIQUETTE))
        {
            return 223;
        }
        else
        {
            return 246;
        }
    }
};

class Story016 : public Story::Base
{
public:
    Story016()
    {
        ID = 16;

        Text = "You clear the pit with one athletic bound. Looking back at the watching courtiers, you see their expectant grins turn to hang-dog expressions of disappointment. Your cocksure mood is deflated a moment later, though, when they come bounding across the pit themselves. There is not a single one of them who is less agile than you.\n\nYou manage to muster a confident smile as you say, \"Well, I've passed your first test.\"\n\n\"That little thing?\2 says the chief courtier with a scornful glance at the pit of coals. \"That wasn't a test -- more a sort of joke to welcome you. The real tests are still to come.\"";

        Image = "images/filler1.png";

        Choices.clear();

        Controls = StandardControls();
    }

    int Continue(Character::Base &player) { return 431; }
};

class Story017 : public Story::Base
{
public:
    std::string PreText = "";

    Story017()
    {
        ID = 17;

        Image = "images/filler1.png";

        Choices.clear();

        Controls = StandardControls();
    }

    void Event(Character::Base &player)
    {
        PreText = "Your eyes can make out nothing in the inky darkness that engulfs you. As you squat down on the guano-crusted floor, there comes a flapping of leathery wings and the first of the bats comes swooping down towards you. You throw up your arm, fending it off with a sob of horror.\n\nAlthough you try to stay alert throughout the night, fatigue finally overcomes you and you sink into a fitful sleep.\n\nYou LOSE 1 Life Point.";

        Character::GAIN_LIFE(player, -1);

        if (player.Life > 0)
        {
            PreText += "\n\nYou awaken hours later to discover that the bats have been gorging on your blood. You are covered with tiny sores where they have chewed into your veins. You huddle miserable against the wall to wait for dawn.";
        }

        Text = PreText.c_str();
    }

    int Continue(Character::Base &player) { return 41; }
};

class Story018 : public Story::Base
{
public:
    Story018()
    {
        ID = 18;

        Text = "You set the skull gently on the dusty ground and take a few paces back, raising your wand.\n\nNecklace of Skulls sees what you are planning and speaks in protest from the inner recesses of his shrine: \"You cannot resurrect him. You do not have that power.\"\n\n\"Raw determination is the basis of all magic,\" you counter. \"My love for my brother will bring him back.\"\n\nThis is the hardest spell you will ever cast. For almost an hour you continue the chant. The wolfish courtiers do not intervene, fearing your power. For his part, Necklace of Skulls is happy to indulge you. He wants to see you fail. You are determined to disappoint him.\n\nSearingly bright light envelops the skull like a phosphoric bubble from which long green sparks go crawling out along the ground. The wand grows hoot in your hand as it channels more magical force than it was ever intended to contain. At last you know you can do no more. Hoarsely uttering the last syllables of the spell, you slump to your knees.\n\nThere is a gasp from the watching courtiers, a howl of spite from the sorcerer. You look up. An hour of staring into the heart of the spell-glare has left a flickering after-image across your vision, but you are sure you can see something stirring. It looks like a man. He rises to his feet and steps towards you. You rub your eyes, then a familiar voice brings tears of joy to them. \"Evening Star,\" he says. Your brother is alive once more!\n\nYou have used up all your sorcery in working this miracle.\n\nYour [SPELLS] skill is lost.\n\nYou gained the codeword VENUS.";

        Image = "images/filler1.png";

        Choices.clear();

        Controls = StandardControls();
    }

    void Event(Character::Base &player)
    {
        if (Character::VERIFY_CODEWORD(player, Codeword::Type::ANGEL))
        {
            Character::REMOVE_CODEWORD(player, Codeword::Type::ANGEL);
        }

        Character::GET_CODEWORDS(player, {Codeword::Type::VENUS});

        Character::LOSE_SKILLS(player, {Skill::Type::SPELLS});
    }

    int Continue(Character::Base &player) { return 42; }
};

class Story019 : public Story::Base
{
public:
    Story019()
    {
        ID = 19;

        Text = "You have time to take a single step towards the black pyramid, then a howl rings out from the shrine -- a howl of such gruesome fury that your sweat runs icy on your brow. The courtiers abandon any semblance of human form and, transforming into wild dogs, scatter with yelps of fear.\n\nThe shadow men dissolve as Necklace of Skulls draws all his power back into himself. There is a rumbling from deep within the pyramid. The roof of the shrine trembles, then splits apart as something rises up through it. The pillars topple; masonry blocks crack open. Necklace of Skulls stands revealed atop the pyramid.\n\nHe is twice the height of a man -- a parody of human form with dead grey features and grotesquely long limbs with too many joints. The eyes are deep sockets under a caul of shrivelled flesh. His robe is sewn from ragged strips of blood-drenched skin; you realize with a shudder they are the flayed skins of men. Around his neck hangs a long chain of gore-spattered skulls, each with living eyes filled with eternal torment.\n\nNecklace of Skulls stands in the rubble of his shrine like a loathsome insect just emerged from a chrysalis. He points a thin finger at you. \"Evening Star,\" he hisses. \"Now you will know the taste of death.\"";

        Image = "images/filler1.png";

        Choices.clear();
        Choices.push_back(Choice::Base(Skill::TARGETING.Name, 204, Skill::Type::TARGETING));
        Choices.push_back(Choice::Base(Skill::SPELLS.Name, 227, Choice::Type::SKILL_ANY, Skill::Type::SPELLS, {Item::Type::MAGIC_WAND, Item::Type::JADE_SWORD}));
        Choices.push_back(Choice::Base("MELEE: Charge straight at him", 250));
        Choices.push_back(Choice::Base("MELEE: Charge at him zigzagging as you run", 273));

        Controls = StandardControls();
    }
};

class Story020 : public Story::Base
{
public:
    Story020()
    {
        ID = 20;

        Text = "There is a raft moored beside the jetty. It starts to move of its own accord once you have climbed aboard and cast off. You are conveyed across the lake. You are shivering because of the deep chill in the air here. The waters stirs sluggishly in the raft's wake, as though on the point of freezing.\n\nGreen light seeps into the sky, which you now see resembles the roof of an unimaginably vast cavern. Perhaps you are gazing up the bedrock on which the living world rests. The notion sends a shudder through your whole body.\n\nYour journey seems to take hours. Other than the faint sloshing of water past the sides of the raft, there is dead silence. At last you catch sight of something ahead. It is a steep pyramid built in the middle of the lake, with steps leading right up from the water to a shrine at the top.\n\nThe raft drifts to a halt beside the steps. You are puzzled. Is this the journey's end? Here, surrounded by leagues of water in all directions?";

        Image = "images/pyramid.png";

        Choices.clear();
        Choices.push_back(Choice::Base(Skill::CUNNING.Name, 12, Skill::Type::CUNNING));
        Choices.push_back(Choice::Base(Skill::CHARMS.Name, 35, Skill::Type::CHARMS));
        Choices.push_back(Choice::Base("Otherwise", 58));

        Controls = StandardControls();
    }
};

class Story021 : public Story::Base
{
public:
    Story021()
    {
        ID = 21;

        Text = "You soon discover that it is possible to dig a tunnel throughout the sand underneath the barrier of flame. It is the work of just a few minutes to return Jade Thunder's wand to him.\n\n\"I wonder why I never thought of that,\" he muses.\"When you have matchless magical power, why bother with ingenuity?\" you suggest.\n\nHe sucks his teeth and gives you a narrow look. \"Oh yes.\"";

        Image = "images/filler1.png";

        Choices.clear();

        Controls = StandardControls();
    }

    int Continue(Character::Base &player) { return 91; }
};

class Story022 : public Story::Base
{
public:
    std::string PreText = "";

    Story022()
    {
        ID = 22;

        Image = "images/filler1.png";

        Controls = StandardControls();
    }

    void Event(Character::Base &player)
    {
        PreText = "Blistering waves of heat rise form the rocks. Dazzled and soaked in sweat, you stumble on under the harsh sun. The ground is cracked and dry. Pinnacles of wind-blasted rock poke up like gnarled fingers into the sky. High above, the vultures circle in a long lazy sweep. They are content to wait until you are too weak to go on. Days and nights become a blur of torment.";

        Choices.clear();

        if (Character::VERIFY_ITEMS(player, {Item::Type::WATERSKIN}))
        {
            Character::GAIN_LIFE(player, -2);

            Character::LOSE_ITEMS(player, {Item::Type::WATERSKIN});

            PreText += "\n\nYou LOSE 2 Life Points. Your waterskin has been emptied.";

            Choices.push_back(Choice::Base("Continue", 69));
        }

        Text = PreText.c_str();
    }

    int Continue(Character::Base &player) { return 322; }
};

class Story023 : public Story::Base
{
public:
    Story023()
    {
        ID = 23;

        Text = "\"Why were you counting the stars anyway?\" you ask him.\n\nBrows like stone lintels tilt in a weighty frown. \"I can't remember,\" he admits at last.\n\n\"But it must have been important?\"\n\n\"I suppose so...\" he murmurs distractedly, still puzzling over the problem.\n\n\"So I ought to get a reward?\"\n\nHis lips tighten like cooling lava. \"Ah, I see. You want payment for your service. Well, I suppose so. What will you take: health, wealth or sound advice?\"\n\nYou ponder the choice for a few moments, prompting him to a grunt of irritation: \"Decide quickly, since I now have another problem to occupy my time!\"";

        Image = "images/giant.png";

        Choices.clear();
        Choices.push_back(Choice::Base("Health", 44));
        Choices.push_back(Choice::Base("Wealth", 67));
        Choices.push_back(Choice::Base("Advice", 90));

        Controls = StandardControls();
    }
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
        Choices.push_back(Choice::Base("Jerk your hand away quickly", 71));
        Choices.push_back(Choice::Base("Slowly reach around with your other hand to seize the tarantula from behind", 94));

        Controls = StandardControls();
    }
};

class Story026 : public Story::Base
{
public:
    Story026()
    {
        ID = 26;

        Text = "With infinite care you delve into the gloomy hole and deftly remove the GOLD DIADEM. Clinging to the side of the tree, you clean off the moss and muck and examine your prize. It is a circlet such as a king or high priest might wear upon his row, patterned with holy sigils and bearing the cruciform symbol of the World Tree in inlaid plaques of jade. Such an item could fetch you a fortune in any market in the world.\n\nClimbing back down to the ground, you sense the stabai hovering close beside you, bending heir elongated skulls closer as they admire your find. \"It is a great treasure, as we promised,\" says one with a trace of envy in her voice. \"Now return our shawl.\"\n\n\"When I'm safely out of these woods, then I'll consider it,\" you snap back. \"Not before.\"\n\nYou got the GOLD DIADEM.";

        Image = "images/filler1.png";

        Choices.clear();

        Controls = StandardControls();
    }

    void Event(Character::Base &player)
    {
        Character::GET_ITEMS(player, {Item::Type::GOLD_DIADEM});
    }

    int Continue(Character::Base &player) { return 390; }
};

class Story027 : public Story::Base
{
public:
    Story027()
    {
        ID = 27;

        Text = "Seeing your look of horror, one of the men prods the fire with a stick and says, \"We're roasting a monkey. Looks pretty gruesome, doesn't it? Tastes delicious, though.\"\n\nFilled with relief to discover that they are not cannibals after all, you join the jungle people in their meal. You soon learn that you have them to thank for the herbs that cured your fever.";

        Image = "images/filler1.png";

        Choices.clear();

        Controls = StandardControls();
    }

    void Event(Character::Base &player)
    {
        Bye = NULL;

        if (!Character::VERIFY_ITEMS(player, {Item::Type::SHAWL}))
        {
            Bye = "When you are ready, you leave the jungle people and resume your journey.";
        }
    }

    int Continue(Character::Base &player)
    {
        if (Character::VERIFY_ITEMS(player, {Item::Type::SHAWL}))
        {
            return 436;
        }
        else
        {
            return 118;
        }
    }
};

class Story028 : public Story::Base
{
public:
    Story028()
    {
        ID = 28;

        Text = "The sun casts its dusty gold light across the treetops to the west, awakening glints in your ceremonial regalia. Summoning up all your courage, you take the final step that carries you off the brink into oblivion. The water rushes up to meet you -- an icy slap, followed by darkness and a roaring silence. You are enfolded in a watery womb, numbly struggling out of the metal accoutrements that are carrying you down into the furthest depths of the well.\n\nBloody darkness thunders through your brain. There is no sign of the sunlit surface of the water overhead. You feel as though you are trapped on the border between sleeping and waking, and in the instant of departing consciousness you know you are not in the bottom of the well any more. You have plunged into the fabled river that leads out of the world of the living. You are on your way to the Deathlands.\n\nYou LOSE 3 Life Points.";

        Image = "images/filler1.png";

        Choices.clear();

        Controls = StandardControls();
    }

    void Event(Character::Base &player)
    {
        Character::GAIN_LIFE(player, -3);
    }

    int Continue(Character::Base &player) { return 119; };
};

class Story029 : public Story::Base
{
public:
    Story029()
    {
        ID = 29;

        Text = "You push through a bank of ferns and pause to get your breath back. You have been walking for hours in the sweltering heat. Moisture trickles down off the leaf canopy, but you cannot even tell if it is rain or just condensation. If only you could get a clear look at the sky, you might be able to tell which way to go.";

        Image = "images/filler1.png";

        Choices.clear();
        Choices.push_back(Choice::Base("Head left from here", 144));
        Choices.push_back(Choice::Base("Go right", 75));
        Choices.push_back(Choice::Base("Go straight on", 98));

        Controls = StandardControls();
    }
};

class Story030 : public Story::Base
{
public:
    Story030()
    {
        ID = 30;

        Text = "Nachan is the busy hub of trade up and down the river, so there are hundreds of boats bobbing up and down at the jetty waiting to unload their wares. You wend your way between bales of grain, fruit, feathers, jade and animal pelts. Most of the trader's vessels are dug-out canoes, but you see one crescent-shaped boat constructed of interwoven reeds. You guess that the owner is not from the wooded country upriver, but must be a native of the fens which lie between here and the coast. Seeing you approach, he straightens up from the task of loading clay pots aboard the boat and winces as he rubs his aching back.\n\n\"Good morning,\" you say to him. \"If you're going downriver, I wonder if you have room for a passenger?\"\n\nHe looks you up and down. \"If you have two cacao to spare.\"\n\n\"Two cacao!\" you cry in outrage. \"That is an exorbitant sum. I will offer--\"\n\nThe fenman holds up his hand to interrupt you. \"Haggling is pointless,\" he says. \"You would take up as much space as two large pitchers, and the profit I make on each pitcher is one cacao. Consequently you must pay a fee of two cacao to compensate me for my loss of earnings if I take you aboard.\"";

        Image = "images/filler1.png";

        Choices.clear();
        Choices.push_back(Choice::Base(Skill::SEAFARING.Name, 332, Skill::Type::SEAFARING));
        Choices.push_back(Choice::Base("Pay him (2 cacao)", 355, Choice::Type::LOSE_MONEY, 2));
        Choices.push_back(Choice::Base("Make your way north on foot", 264));

        Controls = StandardControls();
    }
};

class Story031 : public Story::Base
{
public:
    Story031()
    {
        ID = 31;

        Text = "The folktales of heroes who have ventured into the underworld are as fresh in your mind now as when they were first told to you in infancy. You know that on no account must you part with the JADE BEAD until you reach the crossroads where four coloured paths meet. The two demons are just tying to trick you. You make curt gestures of dismissal and turn away from them.";

        Image = "images/filler1.png";

        Choices.clear();

        Controls = StandardControls();
    }

    int Continue(Character::Base &player) { return 53; }
};

class Story032 : public Story::Base
{
public:
    Story032()
    {
        ID = 32;

        Text = "You give them a puzzled look, your face the very picture of innocence as you reply: \"An OWL? Not at all -- my gift was a fine cloak of quetzal feathers with a jade clasp. I think that fellow over there presented the OWL...\"\n\nYou point to a fretful-looking gentleman who has been pacing along the colonnade for several minutes. You noticed him send in a gift earlier, but he failed to bribe the courtier sufficiently and does not realize he will be kept waiting as a result. He looks up with a bemused half-smile as the guards go striding over to him. The smile soon turns to a look of horror as they start to pummel him with their staves. Cowering under the hail of blows, he goes hobbling off through the crowd of onlookers. Meanwhile you take the opportunity to slink away before anyone finds out the truth.";

        Image = "images/filler1.png";

        Choices.clear();

        Controls = StandardControls();
    }

    int Continue(Character::Base &player) { return 308; }
};

class Story033 : public Story::Base
{
public:
    std::string PreText = "";

    Story033()
    {
        ID = 33;

        Image = "images/filler1.png";

        Choices.clear();

        Choices.push_back(Choice::Base(Skill::SPELLS.Name, 439, Choice::Type::SKILL_ANY, Skill::Type::SPELLS, {Item::Type::MAGIC_WAND, Item::Type::JADE_SWORD}));
        Choices.push_back(Choice::Base(Skill::CUNNING.Name, 428, Skill::Type::CUNNING));
        Choices.push_back(Choice::Base(Item::Descriptions[Item::Type::CHILLI_PEPPERS], 125, Item::Type::CHILLI_PEPPERS));

        Controls = StandardControls();
    }

    void Event(Character::Base &player)
    {
        Type = Story::Type::NORMAL;

        PreText = "You walk into the jaws of the dragon and descend the long tunnel of his throat until the only light is a dim flicker in the gloom far behind you. Hot gases bubble up out of the chambers of his stomach, forcing you to hold your hand over your face as you proceed.\n\nIt gets hotter. You cannot see much of your surroundings, but you reckon that you must now be passing through the dragons bowels. This is the part of his body that lies in the lava at the bottom of the canyon. Such intense temperatures obviously do not bother the dragon, but you are getting weaker by the minute. You stagger on, head swimming from the stinking gases and the burning heat.\n\nYou LOSE 3 Life Points.";

        Character::GAIN_LIFE(player, -3);

        if (player.Life > 0)
        {
            PreText += "\n\nYou realize that at last the passage is beginning to slope upwards again. You are climbing out of the canyon, towards the dragon's other head. Sure enough, the awful heat gradually subsides. When you reach the top of your gruelling ascent, however, a further shock awaits you. The dragon's hind and jaws are closed. You are trapped in here.";

            if (!Character::VERIFY_SKILL(player, Skill::Type::CUNNING) && !Character::VERIFY_SKILL_ANY(player, Skill::Type::SPELLS, {Item::Type::MAGIC_WAND, Item::Type::JADE_SWORD}) && !Character::VERIFY_ITEMS(player, {Item::Type::CHILLI_PEPPERS}))
            {
                PreText += "\n\nYour days will end in the maw of the the dragon.";

                Type = Story::Type::DOOM;
            }
        }

        Text = PreText.c_str();
    }
};

class Story034 : public Story::Base
{
public:
    Story034()
    {
        ID = 34;

        Text = "You moisten a little salt and rub it onto your neck as a precaution. You do not need much, so you can retain the rest of the parcel in case you need it later.\n\nYou gained the codeword SALVATION.";

        Image = "images/filler1.png";

        Choices.clear();

        Controls = StandardControls();
    }

    void Event(Character::Base &player)
    {
        Character::GET_CODEWORDS(player, {Codeword::Type::SALVATION});
    }

    int Continue(Character::Base &player)
    {
        if (Character::VERIFY_ITEMS(player, {Item::Type::LOBSTER_POT}))
        {
            return 81;
        }
        else
        {
            return 57;
        }
    }
};

class Story035 : public Story::Base
{
public:
    Story035()
    {
        ID = 35;

        Text = "Your amulet is held around your neck by a thong, which snaps as you are getting off the raft. You give a yelp of alarm and try to grab the amulet before it falls into the water, but for once luck is not with you and it slips through your fingers. However, instead of sinking without trace the amulet comes to rest just under the surface. You stoop to retrieve it, and in doing so you notice something rather interesting; the stairway does not just lead up to the shrine atop the pyramid; it also leads down beneath the lake.\n\nIs that the route you should take? Or is there some secret you must discover inside the shrine first? As you resecure the amulet around your neck, you ponder your next action.";

        Image = "images/filler1.png";

        Choices.clear();
        Choices.push_back(Choice::Base("Ascend to the shrine", 58));
        Choices.push_back(Choice::Base("Descend into the water", 105));

        Controls = StandardControls();
    }
};

class Story036 : public Story::Base
{
public:
    Story036()
    {
        ID = 36;

        Text = "You trudge on until you come to a well. On a ridge ahead stands a kapok tree with its upper branches surrounded by the mass of water that hangs above the Deathlands. Squinting in the intense unremitting sunlight, you can make out some figures reclining in the shade of its foliage.";

        Image = "images/filler1.png";

        Controls = StandardControls();
    }

    void Event(Character::Base &player)
    {
        Choices.clear();

        if (Character::VERIFY_ITEMS(player, {Item::Type::JADE_BEAD}))
        {
            Choices.push_back(Choice::Base("Discard the JADE BEAD", 174, Choice::Type::LOSE_ITEM, Item::Type::JADE_BEAD));
        }

        Choices.push_back(Choice::Base("Continue", 180));
    }
};

class Story037 : public Story::Base
{
public:
    Story037()
    {
        ID = 37;

        Text = "You have passed two of the sentinels; there are two still to go. The next proves to be a lank bone-white demon with a sharply featured face that gives him an almost serpentine look. He holds a leaf-shaped knife of white onyx in each hand, tilting them like fans as he stoops to peer at you.\n\n\"Your name?\" The words ooze out of his mouth like venom.\n\n\"Evening Star,\" you reply.\n\nHe nods and an ugly lipless smile curls his long mouth, \"Good. Now address me with proper respect, Evening Star. Address me by name.\"\n\nWhat do you think his name is?";

        Image = "images/filler1.png";

        Choices.clear();
        Choices.push_back(Choice::Base("Lord Blood", 84));
        Choices.push_back(Choice::Base("Lord Skull", 129));
        Choices.push_back(Choice::Base("Thunderbolt Laughter", 62));

        Controls = StandardControls();
    }
};

class Story038 : public Story::Base
{
public:
    Story038()
    {
        ID = 38;

        Text = "The servant falls to his knees with his arms clasped around your legs. he is racked with grief for his dead master, and begs you to restore the man to life. He must think you are some kind of miracle-worker. Understandable, since he has just seen you slay a monster that seemed more than a match for any ten normal men.";

        Image = "images/filler1.png";

        Choices.clear();
        Choices.push_back(Choice::Base("[SPELLS] Try to revivify the dead man, though it would probably use up all your magical power", 108, Choice::Type::SKILL_ANY, Skill::Type::SPELLS, {Item::Type::MAGIC_WAND, Item::Type::JADE_SWORD}));
        Choices.push_back(Choice::Base("You can expend your magic using a GOLD DIADEM", 130, Item::Type::GOLD_DIADEM));
        Choices.push_back(Choice::Base("... or a CHALIFE OF LIFE", 153, Item::Type::CHALICE_OF_LIFE));
        Choices.push_back(Choice::Base("Otherwise", 176));

        Controls = StandardControls();
    }
};

class Story039 : public Story::Base
{
public:
    Story039()
    {
        ID = 39;

        Text = "You place it across the pit and walk across. When you reach the other side and turn to retrieve your makeshift bridge, however, the chief courtier whisks it away and snaps it across his knee. \"That was cheating,\" he says ill-temperedly.\n\n\"How was I to know? You didn't mention any rules.\"\n\nHis scowl melts into a sly smile. He is obviously a creature of volatile moods. \"That's the kind of game we play around here. You discover the rules when you break them.\"\n\nHe tosses the broken item aside and takes a great leap which carries him right across the pit. The other courtiers follow with equal agility.";

        Image = "images/filler1.png";

        Choices.clear();

        Controls = StandardControls();
    }

    int Continue(Character::Base &player) { return 431; }
};

class Story040 : public Story::Base
{
public:
    Story040()
    {
        ID = 40;

        Text = "\"I have survived all the ordeals,\" you say to the courtiers, \"and now I demand to see your master -- the sorcerer Necklace of Skulls.\"\n\nThey watch you with smouldering eyes, but the cocksure sneering looks with which they first greeted you are gone now. By passing the five tests you have earned their respect -- perhaps even their fear. As they escort you to the gateway of bones at the rear of the courtyard, the chief courtier studies you with a long sidelong stare before saying, \"You have got further than any mortal I can remember. But our master will crush you as I might crack a flea between my fingernails.\"\n\nWith a hollow rattling noise, the great gates swing inwards to reveal an avenue whose walls slope outwards on either side of you. The black pyramid stands at the far end of the avenue, its steep flanks clad in a block of cold shade that defies the harsh sunlight. The courtiers scatter at a signal you do not hear, rushing off with loping gaits that betray their half-canine ancestry. Climbing stone staircases, they take up positions along the top of the sloping walls.\n\nAs you take a step forward along the avenue, you notice rings set hip on the side walls. It is like the arena in which the sacred ball contest is played, and those stone rings are the goals.\n\nYou round angrily on the chief courtier, who is still standing close at your shoulder. \"What is this?\" you shout. \"I haven't come to play games! You told me I was going to meet Necklace of Skulls at last!\"\n\n\"I am here,\" echoes a sepulchral voice from the depths of the shrine atop the pyramid. \"Now let the game begin.\"";

        Image = "images/ballgame-arena.png";

        Choices.clear();

        Controls = StandardControls();
    }

    int Continue(Character::Base &player) { return 317; }
};

class Story041 : public Story::Base
{
public:
    Story041()
    {
        ID = 41;

        Text = "You are waiting by the door when the dog-like courtiers come to release you in th e morning. The chief is not with them, but later in the day he comes over to where you are sitting at the edge of the plaza and asks: \"How are you enjoying your stay?\"\n\n\"If I were to be candid, I would say your hospitality leaves much to be desired,\" you reply, forcing a note of flippancy into your voice with some effort. \"But I feel this can be excused on the grounds that you received few visitors in these parts.\"\n\n\"On the contrary,\" he says with a broad smile, \"we often have people for dinner.\"\n\nStifling a shudder at this veiled threat, you ask what your next ordeal is to be.\n\nHe glances at the sound, which is already declining in the sky. \"You will shortly discover that for yourself. We call it the House of Knives.\"\n\nSoon afterwards you are taken to the third building. Here the floor is covered with knives of sharp green obsidian. As the sun sets, the knives come to life, springing up to slice at the air expectantly. The door crunches solidly into the place behind you. \"Now,\" the chief courtier calls through it, \"I expect you'll be cut down to size.\"";

        Image = "images/filler1.png";

        Controls = StandardControls();
    }

    void Event(Character::Base &player)
    {
        Choices.clear();

        if (!Character::VERIFY_ITEMS(player, {Item::Type::HAUNCH_OF_VENISON}))
        {
            Choices.push_back(Choice::Base("Use a STONE", 87, Item::Type::STONE));
            Choices.push_back(Choice::Base("Otherwise", 110));
        }
    }

    int Continue(Character::Base &player) { return 64; }
};

class Story042 : public Story::Base
{
public:
    Story042()
    {
        ID = 42;

        Text = "The ball contest is played in every city of the civilized world. It is much more than just a game. Its exponents travel far and wide, earning fame for themselves and glory for their home cities. The priests say that the origins of the contest lie rooted in ancient tradition, and it is said that the playing of each game is like the unfolding of a mighty spell. Portents for the future are read in the outcome. Losers are often sacrificed to the gods.\n\nThe contest involves two players on each side. The aim is to bounce a large rubber ball off the sloping side walls of the arena using only your wrists, elbows and knees. At the same time you have to avoid the opposing players, who are allowed to ram into you with stunning force. You have seen men carried off with the broken necks after a vicious tackle.\n\nThe side walls are marked into zones. You score points for hitting these with the ball, and the winning team is the first to score seven points. Alternatively, you can win an immediate victory by getting the ball to go through one of the stone rings set high up in the middle of each wall. This is a very difficult feat, rarely achieved by even the best players.\n\nThe ritual ball contest has started.";

        Image = "images/filler1.png";

        Choices.clear();

        Controls = StandardControls();
    }

    void Event(Character::Base &player) { player.RitualBallStarted = true; }

    int Continue(Character::Base &player)
    {
        if (Character::VERIFY_CODEWORD(player, Codeword::Type::POKTAPOK))
        {
            return 65;
        }
        else
        {
            return 88;
        }
    }
};

class Story043 : public Story::Base
{
public:
    std::string PreText = "";

    Story043()
    {
        ID = 43;

        Image = "images/filler1.png";

        Choices.clear();

        Choices.push_back(Choice::Base("Attack the sorcerer now", 19));
        Choices.push_back(Choice::Base("Otherwise", 157));

        Controls = StandardControls();
    }

    void Event(Character::Base &player)
    {
        Type = Story::Type::NORMAL;

        PreText = "To your own astonishment as much as anyone else's, the blood ball soars up and unerringly passes through the stone ring set in the middle of the wall. A howl of disbelief rises from the watching courtiers. They sound like hounds at the baying of the moon.\n\nYour brother rushes over to join you. \"Can you feel it, Evening Star?\" he says excitedly. \"The tingle of magic on the air?\"\n\nHe is right. In some strange way your victory worked a spell which now empowers you both with an invigorating surge of energy.";

        if (player.LostSkills.size() > 0)
        {
            PreText += "\n\nYou REGAINED all of the skills you have LOST previously.";

            for (auto i = 0; i < player.LostSkills.size(); i++)
            {
                if (!Character::VERIFY_SKILL(player, player.LostSkills[i].Type))
                {
                    player.Skills.push_back(player.LostSkills[i]);
                }
            }

            player.LostSkills.clear();
        }

        PreText += "\n\nA wail of petulant rage echoes down from the sorcerer's sanctum. \"Apparently he's not happy with the result of the contest,\" you say to Morning Star.";

        Text = PreText.c_str();
    }
};

class Story044 : public Story::Base
{
public:
    Story044()
    {
        ID = 44;

        Text = "The giant adopts a look of furious concentration. He puffs out the huge boulders of his cheeks and screws his eyes tight. A rumbling groan escapes from the deep well of his throat, followed by a spluttering and a single cough like a lava plug being blown out of the ground.\n\nHe opens his mouth and there on his tongue lies a stone jar. \"What's that?\" you ask.\n\n\"Ake it and thee,\" he replies.\n\n\"I beg your pardon?\" you say, lifting the jar to examine it.\n\n'I said, \"Take it and see,\"' he repeats impatiently. When he sees you grimace at the smell of the jar's contents, he adds: \"It's a healing drink. A magical recipe thousands of years old.\"\n\n\"I think it's gone off!\"\n\n\"No, it's supposed to smell like that,\" he says.\n\nThe potion can be drunk once at any time to restore 5 lost Life Points.";

        Image = "images/giant.png";

        Choices.clear();

        Controls = StandardControls();
    }

    void Event(Character::Base &player)
    {
        Character::GET_ITEMS(player, {Item::Type::MAGIC_DRINK});
    }

    int Continue(Character::Base &player) { return 135; }
};

class Story045 : public Story::Base
{
public:
    Story045()
    {
        ID = 45;

        Text = "The golden manikin draws life from the warmth of your hand. Leaping down to the ground, he leaps through the flame barrier and emerge unscathed bearing the wand. You take the wand and give it to the Jade Thunder, who is delighted.\n\nWhen you go to pick up the MAN OF GOLD, you discover he has tunnelled down into the sand. \"I know the legend of that manikin,\" says Jade Thunder. \"It could only be used once.\"\n\n\"Evidently,\" you reply, staring at the mound of sand where the MAN OF GOLD dug down out of sight.";

        Image = "images/filler1.png";

        Choices.clear();

        Controls = StandardControls();
    }

    void Event(Character::Base &player)
    {
        Character::LOSE_ITEMS(player, {Item::Type::MAN_OF_GOLD});
    }

    int Continue(Character::Base &player) { return 91; }
};

class Story046 : public Story::Base
{
public:
    std::string PreText = "";

    Story046()
    {
        ID = 46;

        Image = "images/filler1.png";

        Choices.clear();

        Controls = StandardControls();
    }

    void Event(Character::Base &player)
    {
        PreText = "You have never seen so many stars as fill the desert sky after sunset. The night is full of soft sinister rustlings: snakes gliding across the sand, insects and scorpions scuttling unseen in the darkness. It is as eerie as venturing into the underworld. When the moon rises, it outlines the wind-blasted crags in a ghostly silver glow that makes them look like towering clouds.\n\nBy day you shelter under overhanging rocks -- after first being sure to check that no venomous creatures have used the same patch of shade as a lair. Each evening, as the sun sinks in the west and the terrible heat of the day gives way to the cool of night, you take up your pack and journey on.";

        auto DAMAGE = -3;

        PreText += "\n\n";

        if (Character::VERIFY_ITEMS(player, {Item::Type::WATERSKIN}))
        {
            PreText += "[WATERSKIN] ";

            DAMAGE = -1;
        }
        else if (Character::VERIFY_SKILL(player, Skill::Type::WILDERNESS_LORE))
        {
            PreText += "[WILDERNESS LORE] ";

            DAMAGE += 1;
        }

        PreText += "You LOSE " + std::to_string(-DAMAGE) + " Life Points.";

        Character::GAIN_LIFE(player, DAMAGE);

        if (player.Life > 0)
        {
            if (Character::VERIFY_ITEMS(player, {Item::Type::WATERSKIN}))
            {
                PreText += "\n\nYour WATERSKIN has been emptied.";

                Character::LOSE_ITEMS(player, {Item::Type::WATERSKIN});
            }
            else if (Character::VERIFY_SKILL(player, Skill::Type::WILDERNESS_LORE))
            {
                PreText += "\n\nYour knowledge of WILDERNESS LORE made you tougher than most people.";
            }
        }

        Text = PreText.c_str();
    }

    int Continue(Character::Base &player) { return 69; }
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
        Choices.push_back(Choice::Base("Request a meeting with one of the high priests", 116));
        Choices.push_back(Choice::Base("Ask to see the ancestral treasures of the clan", 138));
        Choices.push_back(Choice::Base("You think a companion would be useful", 162));

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
        Choices.push_back(Choice::Base("Talk to him", 117));
        Choices.push_back(Choice::Base("Hurry off before he gets here", 163));

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

class Story049 : public Story::Base
{
public:
    Story049()
    {
        ID = 49;

        Text = "You reach into the forbidding hole and snatch up the diadem. As you bring it out into the leaf-spattered sunlight however, a baleful roar issues from the interior of the dead tree. You are started by a scaly moss-covered arm that suddenly thrusts forth, groping for you as a voice thunders: \"Who has taken my trinket? A curse be upon that sly long-fingered thief!\"";

        Image = "images/filler1.png";

        Choices.clear();
        Choices.push_back(Choice::Base("[CUNNING] You don't mind losing the SHAWL", 425, Skill::Type::CUNNING, Item::Type::SHAWL));
        Choices.push_back(Choice::Base("Release your grip and fall back off the tree", 400));
        Choices.push_back(Choice::Base("Cling on and risk letting the monstrous arm seize you", 3));

        Controls = StandardControls();
    }
};

class Story050 : public Story::Base
{
public:
    Story050()
    {
        ID = 50;

        Text = "You race off into the undergrowth. The mysterious cannibals make no attempt to stop you, nor do they come after you. They are content to keep your belongings and leave you to the mercy of the forest byways.";

        Image = "images/filler1.png";

        Choices.clear();

        Controls = StandardControls();
    }

    void Event(Character::Base &player)
    {
        Character::LOSE_POSSESSIONS(player);

        Character::GAIN_MONEY(player, player.LostMoney);

        player.LostMoney = 0;
    }

    int Continue(Character::Base &player) { return 118; }
};

class Story051 : public Story::Base
{
public:
    Story051()
    {
        ID = 51;

        Text = "\"Struggling is futile!\" snaps the high priest as his guards rush at you. \"Submit to the will of the gods and at least you will be granted a noble death.\"\n\nYou give a groan of pain as one of the guards slams the butt of his spear across your shoulders. Another kicks you in the stomach as you slump forward.\n\nYou LOSE 2 Life Points.";

        Image = "images/filler1.png";

        Choices.clear();

        Controls = StandardControls();
    }

    void Event(Character::Base &player)
    {
        Character::GAIN_LIFE(player, -2);
    }

    int Continue(Character::Base &player) { return 327; }
};

class Story052 : public Story::Base
{
public:
    Story052()
    {
        ID = 52;

        Text = "A sense of panic begins to well up, churning your thoughts into a confused mixture of fact and fancy. You begin to imagine that you have strayed into the underworld and that the mighty trees surrounding you are no more than the smallest subterranean roots of the fabled Ceiba tree that supports the heavens. You jump in alarm at every tiny sound of scurrying insects or fluttering wings. If you cannot find your way out of the forest soon, our only fate will be madness followed by a slow torturing death by starvation.";

        Image = "images/filler1.png";

        Choices.clear();
        Choices.push_back(Choice::Base("Bear off to the right", 76));
        Choices.push_back(Choice::Base("Continue in the direction you have been walking up till now", 98));
        Choices.push_back(Choice::Base("Go left", 144));

        Controls = StandardControls();
    }
};

class Story053 : public Story::Base
{
public:
    Story053()
    {
        ID = 53;

        Text = "A dry raised path stretches off across the dismal marshland. You set off to find where it leads, and have not been walking long when you come across a tiny wizened man with large round eyes and a very long nose. He is lying by the side of the path in a clump of reeds, gasping with exhaustion. When he catches sight of you he raises his head weakly and says in a high-pitched whine: \"I'm so thirsty. Please give me something to drink...\"";

        Image = "images/filler1.png";

        Choices.clear();
        Choices.push_back(Choice::Base("Go back to the river and fetch some water for him", 284));
        Choices.push_back(Choice::Base("Ignore him and walk past", 307));

        Controls = StandardControls();
    }
};

class Story054 : public Story::Base
{
public:
    Story054()
    {
        ID = 54;

        Text = "You pause to admire the details of the frieze. It is divided into two long panels. In the upper part, two jaunty heroes are shown striding towards a gate where four stern sentinels await them. The faces of the heroes are identical. The lower panel is separated from this by a cornice, and the figures there are shown upside-down as though walking across the bottom of the world. You crane your neck to make out a picture, which the artisans have only half finished colouring in. It depicts a rich man and his servant trudging towards a pair of arches. Their crouched stance and tightly drawn features suggest an air of nervousness which contrasts with the bold manner of the two heroes in the upper mural.\n\nThe priestess smiles at you. \"You're a connoisseur of temple art? We're having to hurry to get it finished before the festivities tomorrow, of course.\"";

        Image = "images/filler1.png";

        Choices.clear();
        Choices.push_back(Choice::Base("Ask her what the scene with the hero-twins represents", 123));
        Choices.push_back(Choice::Base("Ask about the noble and the servant", 146));
        Choices.push_back(Choice::Base("You are more interested in finding out about the festival", 169));

        Controls = StandardControls();
    }
};

class Story055 : public Story::Base
{
public:
    Story055()
    {
        ID = 55;

        Text = "The King is put in a foul temper because of your paltry gift. A group of royal guards emerge from the palace brandishing hard wooden staves and demand to know if you are the one who has insulted the King by presenting him with a one-legged OWL.";

        Image = "images/filler1.png";

        Choices.clear();
        Choices.push_back(Choice::Base(Skill::CUNNING.Name, 32, Skill::Type::CUNNING));
        Choices.push_back(Choice::Base("Fight the guards", 102));
        Choices.push_back(Choice::Base("Make no attempt to resist", 124));

        Controls = StandardControls();
    }
};

class Story056 : public Story::Base
{
public:
    Story056()
    {
        ID = 56;

        Text = "When you answered the riddle, the bead slipped out from under your tongue and fell to the ground. You drop to your knees and start frantically fumbling around for it, then you catch sight of it rolling towards a fissure in the rock. You make a desperate lunge, but your fingers close on thin air. The JADE BEAD rolls into the fissure and is lost to view.";

        Image = "images/filler1.png";

        Choices.clear();

        Controls = StandardControls();
    }

    void Event(Character::Base &player)
    {
        if (Character::VERIFY_ITEMS(player, {Item::Type::JADE_BEAD}))
        {
            Character::LOSE_ITEMS(player, {Item::Type::JADE_BEAD});
        }
    }

    int Continue(Character::Base &player) { return 60; }
};

class Story057 : public Story::Base
{
public:
    Story057()
    {
        ID = 57;

        Text = "Leaving while the creature is asleep would do no good. It would only come looking for you after dark. You must deal with it now.";

        Image = "images/filler1.png";

        Choices.clear();
        Choices.push_back(Choice::Base(Skill::CUNNING.Name, 377, Skill::Type::CUNNING));
        Choices.push_back(Choice::Base(Skill::SPELLS.Name, 11, Choice::Type::SKILL_ANY, Skill::Type::CUNNING, {Item::Type::MAGIC_WAND, Item::Type::JADE_SWORD}));
        Choices.push_back(Choice::Base("Battle with the creature: attack it now while it is still attached to its host", 104));
        Choices.push_back(Choice::Base("Wait until nightfall", 100));

        Controls = StandardControls();
    }
};

class Story058 : public Story::Base
{
public:
    Story058()
    {
        ID = 58;

        Text = "Perhaps you will find the answers to your questions in the shrine. You climb to the top of the steps, pausing for a moment at the threshold of the shrine. The entrance is a block of shadow, dingy with foreboding. But you have no choice. Bowing as every mortal must when in the presence of the gods, you go inside.\n\nIt is a shrine to God of the Pole Star, as you can tell immediately by the striped glyphs on the altar. He is the celestial guide whom all travellers pray to when they have lost their way. You doubt if anyone has ever needed his help as much as you do now.";

        Image = "images/filler1.png";

        Choices.clear();
        Choices.push_back(Choice::Base("Offer INCENSE", 82, Choice::Type::LOSE_ITEM, Item::Type::INCENSE));
        Choices.push_back(Choice::Base("Offer your own life blood (LOSE 1 Life Point)", 82, Choice::Type::LIFE, -1));

        Controls = StandardControls();
    }
};

class Story059 : public Story::Base
{
public:
    Story059()
    {
        ID = 59;

        Text = "The drink proves cool and invigorating. You RECOVER 2 Life Points.\n\nYour companion leads up to the kapok tree, where a group of nobles are resting under the canopy of foliage. They greet him cordially, but you are given a somewhat cooler reception until he explains how you helped him cross the river blood.\n\nIt is a relief to be out of the glaring light of the low underworld sun for a while. Out of the shade, you can see less fortunate souls moving to and fro with their hands pressed to their foreheads, squinting in the eternal sunshine. You settle down with your back to the tree and wait to hear what the nobles have to say.";

        Image = "images/filler1.png";

        Choices.clear();

        Controls = StandardControls();
    }

    void Event(Character::Base &player)
    {
        Character::GAIN_LIFE(player, 2);
    }

    int Continue(Character::Base &player) { return 106; }
};

class Story060 : public Story::Base
{
public:
    Story060()
    {
        ID = 60;

        Text = "The stranger leaps up into the air, displaying a long thin body like a streak of lightning. Suddenly you realize you are not looking at a man at all, but a large iguana. It crouches on the rock, gives you a last lingering glare, and then goes darting off into the mist.\n\nYou trudge on, and gradually the mist breaks up to reveal that the path has become a raised stone causeway which snakes down towards a jetty in the distance. Beyond lies an endless green lake. An icy breeze blows in off the water, making you shiver. As you make your way along the causeway, you notice writhing movement under the thin veils of mist that still lie in the hollows. You stoop for a closer look, then recoil in disgust as you realize that the ground off the causeway consists of filth and mud infested with thousands of maggots.\n\n\"You don't like my pets?\"\n\nYou look up. A bizarre creature is waiting for you a little way down the causeway. You could have sworn it wasn't there a moment before. It has a large globular body supported on three strong clawed legs. Its eyes are bright narrow slits, and as it watches you it runs its tongue greedily across its lips. You realize that to reach the jetty you will have to get past that monster -- or else wade through the mass of wriggling maggots.";

        Image = "images/causeway.png";

        Choices.clear();
        Choices.push_back(Choice::Base(Skill::TARGETING.Name, 217, Skill::Type::TARGETING));
        Choices.push_back(Choice::Base(Item::Descriptions[Item::Type::MAN_OF_GOLD], 241, Item::Type::MAN_OF_GOLD));
        Choices.push_back(Choice::Base("March along the causeway", 194));
        Choices.push_back(Choice::Base("Head directly for the jetty by leaving the causeway and wading through the maggots", 171));

        Controls = StandardControls();
    }

    void Event(Character::Base &player)
    {
        if (Character::VERIFY_ITEMS(player, {Item::Type::JADE_BEAD}))
        {
            Choices[2].Destination = 148;
            Choices[2].Item = Item::Type::JADE_BEAD;
            Choices[2].Type = Choice::Type::ITEM;
        }
        else
        {
            Choices[2].Destination = 194;
            Choices[2].Item = Item::Type::NONE;
            Choices[2].Type = Choice::Type::NORMAL;
        }
    }
};

class Story061 : public Story::Base
{
public:
    Story061()
    {
        ID = 61;

        Text = "Grey clouds of fury darken the black pools of the sentinel's eyes. He lets his gory mouth drop open in a long appalling howl of fury that sounds like the heavens cracking in two. You drop cowering to the ground, so terrified that every muscle in your body loses all strength.\n\nAt last, like a storm, the awful sound passes. You uncurl yourself and glance timidly up. The sentinel has lost interest in you, having voiced his displeasure. He is once more staring directly ahead across the passage, giving you no more attention than he would give to an insect.\n\nStill dazed, you lope on along the passage. It is only now that you realize how the sentinel's howl has addled your wits.\n\nPray to all the gods that your single skill will be enough to see you through.";

        Image = "images/filler1.png";

        Choices.clear();
        Choices.push_back(Choice::Base("Select which SKILLS to lose", 37, Choice::Type::LOSE_SKILLS, 1));

        Controls = StandardControls();
    }
};

class Story062 : public Story::Base
{
public:
    Story062()
    {
        ID = 62;

        Text = "He gives a wild shriek of rage and drives both knives towards your breast. The attack is so fast that you have no time even to flinch. At the last instant, the sentinel pulls his blows so that instead of impaling you the tips of the blades just prick your skin. You gulp and look down. Two bright drops of blood are trickling down your chest. This is not a normal injury, however. The wounds inflicted by Lord Blood's knives can never be healed.\n\nYou PERMANENTLY LOSE 2 Life Points.";

        Image = "images/filler1.png";

        Choices.clear();

        Controls = StandardControls();
    }

    void Event(Character::Base &player)
    {
        player.MAX_LIFE_LIMIT -= 2;

        Character::GAIN_LIFE(player, -2);
    }

    int Continue(Character::Base &player) { return 84; }
};

class Story063 : public Story::Base
{
public:
    std::string PreText = "";

    Story063()
    {
        ID = 63;

        Image = "images/filler1.png";

        Choices.clear();

        Controls = StandardControls();
    }

    void Event(Character::Base &player)
    {
        PreText = "Your strongest leap carries you to the far side of the pit -- nearly. You teeter on the brink with just your toes on solid ground, arms spinning crazily in a vain attempt to save yourself. You fall back with a cry of alarm which turns into a scream of tortured pain as you land on the burning coals.";

        Character::GAIN_LIFE(player, -3);

        if (player.Life > 0)
        {
            PreText += "\n\nThe smell of your own sizzling flesh gives you the burst of frantic strength you need to scramble up out of the pit. You stagger on to the end of the passage to find the courtiers waiting for you. They have wide canine smirks on their faces. \"I hope you appreciated our little jest,\" the chief courtier says. \"Now the real tests begin.\"";
        }

        Text = PreText.c_str();
    }

    int Continue(Character::Base &player) { return 431; }
};

class Story064 : public Story::Base
{
public:
    Story064()
    {
        ID = 64;

        Text = "You wedge yourself into a corner of the hall and watch the hovering knives. As they come sweeping towards you, you tear off a hunk of meat and throw it to them. They fall on it, shredding it quickly with stabbing blows, then retreat to float around in the centre of the hall. After a while they start to approach, and again you are able to distract them with a scrap of meat.\n\nThis continues throughout the knight. You get no sleep but at least you have kept the enchanted knives from your flesh. You have used up the last of the HAUNCH OF VENISON and are waiting nervously for the next assault of the knives, when they suddenly drop lifeless to the floor with the advent of morning.\n\nThe courtiers cannot disguise their ill temper when they open the door to find you unscathed. \"Your luck runs out tonight,\" snaps one. \"That's when you must enter the House of cold.\"";

        Image = "images/filler1.png";

        Choices.clear();

        Controls = StandardControls();
    }

    void Event(Character::Base &player)
    {
        Character::LOSE_ITEMS(player, {Item::Type::HAUNCH_OF_VENISON});
    }

    int Continue(Character::Base &player) { return 132; }
};

class Story065 : public Story::Base
{
public:
    Story065()
    {
        ID = 65;

        Text = "You think it might be worth going for a long shot along the arena as soon as the game starts. If you are lucky you might score a point straight away, thus gaining an advantage. In subsequent rounds you know you will have to play more cautiously -- perhaps even allowing the attacking enemy player to get past you so that you can close in on the defensive player.";

        Image = "images/filler1.png";

        Choices.clear();

        Controls = StandardControls();
    }

    int Continue(Character::Base &player) { return 88; }
};

class Story066 : public Story::Base
{
public:
    Story066()
    {
        ID = 66;

        Text = "On a signal from the chief courtier, both teams return to the end zones of the arena. It is your turn to serve again. You raise the ball and consider your best tactics. The first team to reach seven points wins the contest.";

        Image = "images/filler1.png";

        Choices.clear();
        Choices.push_back(Choice::Base("Go for a long shot", 89));
        Choices.push_back(Choice::Base("You prefer to be cautious and go for a safe point", 112));

        Controls = StandardControls();
    }
};

class Story067 : public Story::Base
{
public:
    Story067()
    {
        ID = 67;

        Text = "The giant blinks. Each time he does, there is a whooshing sound in thin air and a cacao drops from nowhere into your hand. He continues doing this until you have another twenty cacao.\n\n\"A good trick,\" you say, smiling and nodding in the hope that this will encourage him to continue.\n\nHe sighs wearily. \"Once my magic was much grater than that. I fear I have squandered eternity.\"\n\n\"By making a tally of all the stars?\" you say as you slip the cacao into your money-pouch. \"What could be more worthwhile!\"\n\nA streak of light flickers across the heavens. \"what was that?\" says the giant grimly.\n\n\"A falling star,\" you reply. \"Better reduce the total by one. Well, I'll be saying goodbye.\"\n\nYou GAIN 20 cacao.";

        Image = "images/giant.png";

        Choices.clear();

        Controls = StandardControls();
    }

    void Event(Character::Base &player)
    {
        Character::GAIN_MONEY(player, 20);
    }

    int Continue(Character::Base &player) { return 135; }
};

class Story068 : public Story::Base
{
public:
    std::string PreText = "";

    Story068()
    {
        ID = 68;

        Image = "images/filler1.png";

        Choices.clear();

        Controls = StandardControls();
    }

    void Event(Character::Base &player)
    {
        PreText = "You dive through the magical flames. A cry of agony escapes your lips as you are terribly burned.";

        auto DAMAGE = -7;

        PreText += "\n\n";

        if (Character::VERIFY_SKILL(player, Skill::Type::CHARMS))
        {
            PreText += "[CHARMS] ";

            DAMAGE = -3;
        }

        PreText += "You LOSE " + std::to_string(-DAMAGE) + " Life Points.";

        Character::GAIN_LIFE(player, DAMAGE);

        if (player.Life > 0)
        {
            PreText += "\n\nLuckily, you survived. The flame barrier dies down now that you have broken the spell. As you are still recovering from the wave of pain, Jade Thunder steps forward and eagerly takes the wand from your hand.";
        }

        Text = PreText.c_str();
    }

    int Continue(Character::Base &player) { return 91; }
};

class Story069 : public Story::Base
{
public:
    Story069()
    {
        ID = 69;

        Text = "Cliffs rise in front of you, and you make your way along them until you find a long shoulder of rock by which you are able to scale to the top.\n\nYou have gone only a little further when you hear a distant keening noise. It sounds like the wind, but you do not feel even a breath of air in the sultry stillness. Then you notice half a dozen long plumes of dust moving along the ground in your direction. Above each dust-plume is a dark twisting funnel of air. Whirlwinds -- and they are bearing straight down on you. Superstitious dread crawls up your spine. You recall tales of the demons of the desert, who rip men limb from limb with the fury of their whirlwinds.";

        Image = "images/filler1.png";

        Choices.clear();
        Choices.push_back(Choice::Base(Skill::FOLKLORE.Name, 345, Skill::Type::FOLKLORE));
        Choices.push_back(Choice::Base(Skill::SPELLS.Name, 92, Choice::Type::SKILL_ANY, Skill::Type::SPELLS, {Item::Type::MAGIC_WAND, Item::Type::JADE_SWORD}));
        Choices.push_back(Choice::Base("Stand ready to fight the demons off", 115));
        Choices.push_back(Choice::Base("Run back towards the cliffs", 137));

        Controls = StandardControls();
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

    void Event(Character::Base &player)
    {
        Character::GET_UNIQUE_ITEMS(player, {Item::Type::LETTER_OF_INTRODUCTION});
    }

    int Continue(Character::Base &player) { return 93; }
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
        Choices.push_back(Choice::Base("Talk to him", 117));
        Choices.push_back(Choice::Base("Hurry off before he gets here", 163));

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

class Story072 : public Story::Base
{
public:
    Story072()
    {
        ID = 72;

        Text = "A wave of dizziness warns you that your wound is becoming infected. You stop to gather puffballs. Their spores act as an antidote to fever. Finding a wild bees' nest, you mix the spores with honey to take away the dry noxious taste and gulp the mixture down. It is unpleasant, but it seems to do the trick.";

        Image = "images/filler1.png";

        Choices.clear();

        Controls = StandardControls();
    }

    int Continue(Character::Base &player) { return 118; }
};

class Story073 : public Story::Base
{
public:
    std::string PreText = "";

    Story073()
    {
        ID = 73;

        Image = "images/filler1.png";

        Choices.clear();

        Controls = StandardControls();
    }

    void Event(Character::Base &player)
    {
        PreText = "With one bound you cross the veranda and snatch up your belongings. The cannibals are at first taken by surprise, but when you launch yourself at them with a bellow of righteous wrath they take up their weapons and stand ready to fight.";

        auto DAMAGE = -6;

        PreText += "\n\n";

        if (Character::VERIFY_SKILL_ANY(player, Skill::Type::SWORDPLAY, {Item::Type::SWORD, Item::Type::JADE_SWORD}))
        {
            DAMAGE = -2;

            PreText += "[SWORDPLAY] ";
        }
        else if (Character::VERIFY_SKILL(player, Skill::Type::UNARMED_COMBAT))
        {
            PreText += "[UNARMED COMBAT] ";

            DAMAGE = -4;
        }

        PreText += "You LOSE " + std::to_string(-DAMAGE) + " Life Points.";

        Character::GAIN_LIFE(player, DAMAGE);

        Text = PreText.c_str();
    }

    int Continue(Character::Base &player) { return 433; }
};

class Story074 : public Story::Base
{
public:
    Story074()
    {
        ID = 74;

        Text = "You glance back to reassure yourself that the demons are not going to abandon you here. \"We will wait,\" says one with a raw-gummed leer.\n\n\"Take your time,\" cackles the other, nodding in grisly encouragement.\n\nYou brace yourself on the edge of the crevice and peer within. As your eyes adjust to the darkness, you see a narrow tunnel leading to a chamber inside the rock. Something gleams dully in the grey light. The smell is of rotting things: dank leaf mould and stagnant slime.";

        Image = "images/filler1.png";

        Choices.clear();
        Choices.push_back(Choice::Base("Return to the canoe", 258));
        Choices.push_back(Choice::Base("[ROGUERY] Sneak into the tunnel", 282, Skill::Type::ROGUERY));
        Choices.push_back(Choice::Base("Enter into the tunnel", 140));

        Controls = StandardControls();
    }
};

class Story075 : public Story::Base
{
public:
    Story075()
    {
        ID = 75;

        Text = "You reach a clearing where one of the huge primeval trees has toppled, leaving a temporary rent in the leaf canopy. The great trunk lies like a fallen titan across the leaf littler. Already flowers are blossoming in its bark, their tendrils sucking nutriment out of the decaying wood, and great flanges of fungus thrive in its dank crevices. Other trees will son grow their branches across to exploit the sunlight, but for the moment the sky is revealed in a patch of glorious blue that makes your heart soar. You watch the sun slowly decline from its zenith, slanting off across the treetops to your left. Is this information enough to let you find your way out of the forest?";

        Image = "images/filler1.png";

        Choices.clear();
        Choices.push_back(Choice::Base("Go straight ahead", 52));
        Choices.push_back(Choice::Base("Go left from here", 412));
        Choices.push_back(Choice::Base("Go right from here", 121));

        Controls = StandardControls();
    }
};

class Story076 : public Story::Base
{
public:
    Story076()
    {
        ID = 76;

        Text = "Leaping to the back of the boat, you pick up the paddle and use it to steer over towards the cavern wall, where the current is not so fast-moving. After a short distance you find a side tunnel and see the gleam of daylight at the far end. Paddling along it, you emerge into the open under an overcast sky the colour of dead skin. The river has become no more than a muddy trickle winding through sickly grey marshland. A dreary landscape of sour white clay and colourless rushes stretches far off into the distance. There is a foul rancid odour in the air.\n\nYou put in at a rotting wooden jetty and tether the boat.";

        Image = "images/filler1.png";

        Choices.clear();
        Choices.push_back(Choice::Base("Keep the PADDLE", 53, Choice::Type::GET_ITEM, Item::Type::PADDLE));
        Choices.push_back(Choice::Base("Leave it", 53));

        Controls = StandardControls();
    }
};

class Story077 : public Story::Base
{
public:
    Story077()
    {
        ID = 77;

        Image = "images/filler1.png";

        Choices.clear();

        Controls = StandardControls();
    }

    int Background(Character::Base &player)
    {
        if (Character::VERIFY_CODEWORD(player, Codeword::Type::POKTAPOK) && !Character::VERIFY_SKILL(player, Skill::Type::ETIQUETTE))
        {
            return 215;
        }
        else
        {
            return -1;
        }
    }

    void Event(Character::Base &player)
    {
        Choices.clear();

        if (Character::VERIFY_SKILL(player, Skill::Type::ETIQUETTE))
        {
            Text = "You are naturally recognized and welcomed as a noble of Koba.";
        }
        else
        {
            Text = "You were not granted an audience with the king.";

            Choices.push_back(Choice::Base("Present the king with a lavish gift", 238));
            Choices.push_back(Choice::Base("Give up any hope of being granted an audience", 262));
        }
    }

    int Continue(Character::Base &player) { return 192; }
};

class Story078 : public Story::Base
{
public:
    std::string PreText = "";

    Story078()
    {
        ID = 78;

        Image = "images/filler1.png";

        Choices.clear();

        Controls = StandardControls();
    }

    void Event(Character::Base &player)
    {
        PreText = "The courtier returns some time later and tells you that the King is pleased with your gift. A group of royal servants is assigned to take you to the house of a minor nobleman, Lord Fire Serpent. He proves to be a bearded old warrior with a scar across his lip that gives him a rather ferocious appearance. But he greets you cordially when the servants explain that you are a favourite of the King, who commands that you be shown every hospitality.\n\nClapping his hands, Fire Serpent summons his wife, who brings you a jug of spiced cocoa, then gestures for you to sit beside him. \"Tomorrow is the festival commemorating the old King's departure to the next life,\" he says. \"Is that why you have made the journey from Koba?\"\n\nSipping your cocoa, you explain that you are on a quest which will take you much further west than this. Fire Serpent nods interestedly and has food brought.\n\nYou RECOVER 1 Life Point.";

        Character::GAIN_LIFE(player, 1);

        Choices.clear();

        if (!Character::VERIFY_CODEWORD(player, Codeword::Type::PSYCHODUCT))
        {
            PreText += "\n\nYou spend a restful night at Fire Serpent's home. The next day, you decide to...";

            Choices.push_back(Choice::Base("Head north", 30));
            Choices.push_back(Choice::Base("West", 8));
            Choices.push_back(Choice::Base("Stay for the festivities", 416));
        }

        Text = PreText.c_str();
    }

    int Continue(Character::Base &player)
    {
        return 331;
    }
};

class Story079 : public Story::Base
{
public:
    Story079()
    {
        ID = 79;

        Text = "Seeing that you have no intention of paying his fee, Kawak closes his jaws with a crocodilian snap that sends shockwaves rattling through the ground underfoot. He thinks he has thwarted you, but the MAN OF GOLD is more powerful than any dragon. You warm the manikin in your hands and set it down in from of Kawak's stubbornly sealed maw. Kawak squints at it warily along the length of his snout, then his glistening eyes widen in shocked recognition. The MAN OF GOLD does not even need to do anything. Kawak immediately opens his mouth, extending his ridged tongue like a carpet laid before an honoured guest. \"Enter then, mortal, if you must,\" he growls grudgingly.\n\nYou stoop to retrieve the MAN OF GOLD, but it suddenly darts away and leaps off into the abyss.";

        Image = "images/filler1.png";

        Choices.clear();

        Controls = StandardControls();
    }

    void Event(Character::Base &player)
    {
        Character::LOSE_ITEMS(player, {Item::Type::MAN_OF_GOLD});
    }

    int Continue(Character::Base &player) { return 33; }
};

class Story080 : public Story::Base
{
public:
    Story080()
    {
        ID = 80;

        Text = "You arrive at the edge of a canyon. Choking yellow vapour rises from the depths, obscuring a sullen fiery light from far below. You can hear distant rumblings, leading you to imagine a river of lava burning beneath the sulphur clouds.\n\nThere are thin spires of rock poking up out of the vapour at regular intervals almost two metres apart, leading in a straight line to the far side of the canyon. By jumping form one to another it might be possible to get across, but they would make precarious stepping-stones: the top of each spire is a flattened area no bigger than the palm of your hand.";

        Image = "images/filler1.png";

        Controls = StandardControls();
    }

    void Event(Character::Base &player)
    {
        Choices.clear();

        if (!Character::VERIFY_SKILL(player, Skill::Type::FOLKLORE) && !Character::VERIFY_CODEWORD(player, Codeword::Type::PAKAL))
        {
            Choices.push_back(Choice::Base("Cross the canyon by leaping from one spire to the next", 147));
            Choices.push_back(Choice::Base("Use a BLOWGUN to cross more safely", 170, Item::Type::BLOWGUN));
        }
    }

    int Continue(Character::Base &player)
    {
        if (Character::VERIFY_SKILL(player, Skill::Type::FOLKLORE))
        {
            return 286;
        }
        else if (Character::VERIFY_CODEWORD(player, Codeword::Type::PAKAL))
        {
            return 309;
        }
        else
        {
            return -1;
        }
    }
};

class Story081 : public Story::Base
{
public:
    Story081()
    {
        ID = 81;

        Text = "You settle down and wait for night to fall. As the sun dips across the network of canals and trees to the west, the pitcher on the woman's shoulder starts to stir as if of its own accord. It falls away to reveal what you expected: a second head protruding from her neck. The eyes snap open and fix on you, and the head's mouth drops open in a long hissing snarl. Long strands of black hair extend rapidly from it like tentacles -- some of them up to two metres long. These form into thin matted stalks like insectoid legs which probe the ground, preparing to support the creature's weight. There is a grisly sucking sound as the head pulls itself free of the sleeping woman's neck.\n\nIt comes scuttling forward eagerly on its limbs of twined hair and leaps up onto your neck, intending to make you its new host, but you are ready for it. The coating of salt cause it to recoil and it drops to the ground, momentarily helpless. You seize your chance to stuff it into the LOBSTER POT, which you weight with stones before throwing it into the water. \"And good riddance,\" you say as it sinks to a final resting-place on the river bed.";

        Image = "images/filler1.png";

        Choices.clear();

        Controls = StandardControls();
    }

    void Event(Character::Base &player)
    {
        Character::LOSE_ITEMS(player, {Item::Type::LOBSTER_POT});
    }

    int Continue(Character::Base &player) { return 398; }
};

class Story082 : public Story::Base
{
public:
    Story082()
    {
        ID = 82;

        Text = "The god accepts your sacrifice and reveals the true path to you. You hear no words. Suddenly the knowledge is in your mind, where before there was confusion. You know that you must descend the steps back down to the lake -- and then keep going. The route to the Deathlands lies under the water.\n\nBacking out of the shrine, you respectfully retreat one step at a time until you reach the water's edge. Now that you look closely, you can see that the stairway does indeed continue down into the icy green murk.";

        Image = "images/filler1.png";

        Choices.clear();

        Controls = StandardControls();
    }

    int Continue(Character::Base &player) { return 105; }
};

class Story083 : public Story::Base
{
public:
    Story083()
    {
        ID = 83;

        Text = "Your companion leads you past miserable souls who are doomed to mill about for ever in the blazing sunshine, eyes downcast and hands pressed to their foreheads. As you approach the kapok tree, you see that the figures sitting under it are living skeletons. Their bones are green with algae and moss, and creepers and insects twine through their open joints. One raises a grinning face made even more grotesque by a brilliantly patterned butterfly sitting above his eyeless sockets. It is strange juxtaposition of the imagery of life and death.\n\nYour companion tells you that the denizens of the Deathlands are nobles like himself who, because of their status, are privileged to rest in the shade of the tree. Now you see that he too is changing. The appearance of flesh and sinew is dropping away to reveal another of the emerald skeletons with its covering of foliage and wildlife.";

        Image = "images/filler1.png";

        Choices.clear();

        Controls = StandardControls();
    }

    int Continue(Character::Base &player)
    {
        if (Character::VERIFY_SKILL_ANY(player, Skill::Type::SPELLS, {Item::Type::MAGIC_WAND, Item::Type::JADE_SWORD}))
        {
            return 128;
        }
        else
        {
            return 151;
        }
    }
};

class Story084 : public Story::Base
{
public:
    Story084()
    {
        ID = 84;

        Text = "You reach the last of the sentinels -- a giant hunched figure with eyes like fiery mirrors. He crooks one of his taloned fingers and beckons you closer. \"Tell me,\" he says in a rasping whisper, \"by what name am I called?\"\n\nThis is your last test. Get past this demon unscathed and you can escape into the fresh air of the living world.";

        Image = "images/filler1.png";

        Choices.clear();
        Choices.push_back(Choice::Base("Address him as Lord Skull", 336));
        Choices.push_back(Choice::Base("Call him Thunderbolt Laughter", 349));

        Controls = StandardControls();
    }
};

class Story085 : public Story::Base
{
public:
    std::string PreText = "";

    Story085()
    {
        ID = 85;

        Image = "images/filler1.png";

        Choices.clear();

        Bye = "Days turn to weeks. At last you catch sight of the town of Shakalla in the distance, its pyramids trembling in a haze of heat and dust. Beyond it lies a grim grey shadow: the desert, stretched like a basking serpent along the edge of the world.";

        Controls = StandardControls();
    }

    void Event(Character::Base &player)
    {
        PreText = "Food will be hard to come by in the arid sierra, so you make sure to pluck fruits from the abandoned orchards lining the first few kilometres of the causeway. The causeway dwindles to a stony road, then a dirt track, and finally you are trudging through open country.\n\nYour fruit soon gives out but in the baking summer heat it is lack of water, not food, that is your main concern.";

        if (!Character::VERIFY_SKILL(player, Skill::Type::WILDERNESS_LORE) && !Character::VERIFY_ITEMS(player, {Item::Type::WATERSKIN}))
        {
            PreText += "\n\nYou LOSE 1 Life Point.";

            Character::GAIN_LIFE(player, -1);
        }

        Text = PreText.c_str();
    }

    int Continue(Character::Base &player) { return 321; };
};

class Story086 : public Story::Base
{
public:
    Story086()
    {
        ID = 86;

        Text = "The arrangement of beams reminds you of a trap you were once asked to devise to foil tomb robbers. It is all down to a question of basic stress and strain: identifying which beams are taking the weight of the wall, and which you can safely remove without disturbing anything.\n\nIt takes you the better part of an hour, but at last you clear enough space to pick your way through the far end of the passage. You find the courtiers already waiting for you there, crouching around the sides of the wide sunlit courtyard. The chief courtier leaps to his feet as he sees you.\n\n\"What kept you?\" he said. \"If you found that little puzzle of ours difficult, you're going to have real trouble with tests to come.\"";

        Image = "images/filler1.png";

        Choices.clear();

        Controls = StandardControls();
    }

    int Continue(Character::Base &player) { return 431; }
};

class Story087 : public Story::Base
{
public:
    Story087()
    {
        ID = 87;

        Text = "You wedge yourself into a corner of the hall and watch the hovering knives with a wary eye. As they come sweeping through the air towards you, you thrust out the stone. Sparks fly as the first of the knives strikes it, chipping its edge of volcanic glass.\n\nThe knives go darting away like startled birds, retreating to float around uncertainly in the centre of the hall. After a while they seem to recover themselves and again start to approach. Again you strike out with the stone, blunting one of the knives and sending them veering away -- but this time the attack took a chip out of the stone.\n\nThis continues throughout the night. You get no sleep, but at least you have kept the enchanted knives from your own flesh. Your stone shrinks to the size of a pebble, and finally a concerted attack by the knives shatters it entirely. You are waiting nervously for the knives' next assault when they suddenly drop lifeless to the floor with the coming of the dawn.\n\nThe courtiers cannot disguise their ill temper when they open the door to find you unscathed. \"You're a cool customer,\" snaps one, \"so you're probably looking forward to the House of Cold tonight.\"";

        Image = "images/filler1.png";

        Choices.clear();

        Controls = StandardControls();
    }

    void Event(Character::Base &player)
    {
        Character::LOSE_ITEMS(player, {Item::Type::STONE});
    }

    int Continue(Character::Base &player) { return 132; }
};

class Story088 : public Story::Base
{
public:
    Story088()
    {
        ID = 88;

        Text = "The chief courtier comes forwards and puts the ball into your hands. \"So we get to launch the first round?\" you say. \"Very sporting.\"\n\n\"We are nothing if not magnanimous,\" he replies with a vaunting leer. \"Later, when you have lost, we will be equally be generous in dividing your carcass.\"\n\nYou watch him dart back to the sidelines. At the other end of the arena, the two shadow men stand ready.\n\n\"Begin,\" commands Necklace of Skulls.\n\nYou throw the ball against the side wall and run forward to intercept it on the rebound. The nearer of the shadow men charges towards you.";

        Image = "images/filler1.png";

        Choices.clear();
        Choices.push_back(Choice::Base("Tackle him head-on", 111));
        Choices.push_back(Choice::Base("Weave around him towards the rear shadow man", 133));
        Choices.push_back(Choice::Base("Try to score a point immediately", 156));

        Controls = StandardControls();
    }
};

class Story089 : public Story::Base
{
public:
    Story089()
    {
        ID = 89;

        Text = "You aim the ball high up on the wall, so that it strikes the angle where the slope meets the vertical. As it rebounds in a long arc that carries it far out across the arena, you run forward and deflect it against the high-scoring zone on the opposite wall. The ball ricochets off towards your opponents, who leap in to seize possession.";

        Image = "images/filler1.png";

        Choices.clear();

        Controls = StandardControls();
    }

    void Event(Character::Base &player)
    {
        Character::SCORE(player, player.Ticks, 2);
    }

    int Continue(Character::Base &player)
    {
        if (player.Ticks > 6)
        {
            return 134;
        }
        else if (Character::VERIFY_CODEWORDS_ANY(player, {Codeword::Type::SHADE, Codeword::Type::ANGEL}))
        {
            return 19;
        }
        else
        {
            return 181;
        }
    }
};

class Story090 : public Story::Base
{
public:
    Story090()
    {
        ID = 90;

        Text = "\"Buy a waterskin,\" the giant tells you.\n\n\"Others have suggested the same thing.\"\n\n\"Buy two, then. And a knife or sword. There is a four-headed serpent in the desert, and its one weak spot is at the branching of its four necks. You'll need to get close to land a killing blow, and my advice is to pretend to retreat at first. Dodge back a couple of times and the serpent will rush at you headlong -- er, heads-long. Then you can slay it.\"\n\n\"What about just avoiding it?\"\n\n\"Then you wouldn't get a drop of its blood -- a substance like sap, which hardens into rubber.\"\n\n\"And what's the good of that?\"\n\n\"The rubber ball will help you in the Necklace of Skull's contest. The ball contest is an ancient ritual which he uses to humiliate and weaken his foes, but by scoring a daring victory you can exploit the contest's magic for yourself.\"\n\n\"How do I kill Necklace of Skulls?\"\n\nHe snorts contemptuously. \"You mortals are so predictable. There are greater victories than revenge. Do you know the ultimate triumph?\"\n\nYou can think of several replies, but you doubt if any of them are what he is driving at. \"No.\"\n\n\"The ultimate triumph is to be greater than your enemy,\" he says. \"There, that is my best advice.\"";

        Image = "images/giant.png";

        Choices.clear();

        Controls = StandardControls();
    }

    int Continue(Character::Base &player) { return 135; }
};

class Story091 : public Story::Base
{
public:
    Story091()
    {
        ID = 91;

        Text = "Jade Thunder goes down to the water's edge and sweeps his wand in a grand magical gesture. The water immediately in front of him becomes as smooth and flat as a sheet of glass. You blink in amazement as the effect stretches off into the distance, leaving a glassy causeway through the waves.\n\nYou test your weight on the causeway. It is solid. \"Neat trick,\" you say, impressed.\n\n\"I used to be quite famous in my heyday.\" Jade Thunder starts out along the causeway.\n\n\"Can't I join you?\" you call after him.\n\n\"Not on this path. But if you care to sail south to the mainland, look along the coast for a giant who's buried to his neck in the sand. He has been counting stars since the dawn of time. Tell him the true number, which is one hundred thousand million and seven, and he will grant you one wish.\"\n\nYou watch him walk of towards the horizon, then go to rejoin the others at the ship. You are amazed to discover that instead of the flimsy vessel in which you set sail, you now have a magnificent craft of green-lacquered kikche wood with magical sails that can never lose the wind. You climb aboard and put out to sea, but now you must decide.";

        Image = "images/filler1.png";

        Choices.clear();
        Choices.push_back(Choice::Base("Go east to Tahil", 300));
        Choices.push_back(Choice::Base("Go south as the wizard suggested", 136));

        Controls = StandardControls();
    }

    void Event(Character::Base &player)
    {
        if (Character::VERIFY_CODEWORD(player, Codeword::Type::SAKBE))
        {
            Choices[1].Destination = 114;
            Choices[1].Type = Choice::Type::CODEWORD;
            Choices[1].Codeword = Codeword::Type::SAKBE;
        }
        else
        {
            Choices[1].Destination = 136;
            Choices[1].Type = Choice::Type::NORMAL;
            Choices[1].Codeword = Codeword::Type::NONE;
        }
    }
};

class Story092 : public Story::Base
{
public:
    std::string PreText = "";

    Story092()
    {
        ID = 92;

        Image = "images/filler1.png";

        Choices.clear();
        Choices.push_back(Choice::Base("Do battle with the demons", 115));
        Choices.push_back(Choice::Base("Run away from them", 137));

        Controls = StandardControls();
    }

    void Event(Character::Base &player)
    {
        PreText = "You send a surge of occult power to drive the whirlwinds back, but there are too many of them. Here they are in their element, drawing strength from the sand and the rocks and the dry desert air. They penetrate your barrier of spells and come roaring forward, ripping at your body with invisible hands.";

        Type = Story::Type::DOOM;

        if (Character::VERIFY_SKILL(player, Skill::Type::CHARMS))
        {
            Character::GAIN_LIFE(player, -2);

            if (player.Life > 0)
            {
                Type = Story::Type::NORMAL;
            }
        }
        else
        {
            PreText += "\n\nYou were KILLED at once.";
        }

        Text = PreText.c_str();
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

        Shop = {
            {Item::Type::WATERSKIN, 2},
            {Item::Type::ROPE, 3},
            {Item::Type::FIREBRAND, 2},
            {Item::Type::POT_OF_DYE, 2},
            {Item::Type::CHILLI_PEPPERS, 1}};

        Controls = ShopControls();
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
        Choices.push_back(Choice::Base("Talk to him", 117));
        Choices.push_back(Choice::Base("Hurry off before he gets here", 163));

        Controls = StandardControls();
    }

    void Event(Character::Base &player)
    {
        PreText = "The spider's bristly limbs send a shiver through you as they slowly probe your outstretched hand. It takes every shred of nerve to remain motionless while you carefully reach around behind it with your other hand. Its multiple eyes gleam horribly, full of the ruthless intensity of the predator. It looks like a demon carved from polished mahogany, more nightmarish than any image on the walls of the Temple of Death.\n\nAs you take hold of it, it starts to twitch its legs furiously. With a sob of revulsion, you hurl it away. It falls with an audible thud somewhere off among the trees, but then a stab of pain convulses your hand. Did it bite you after all? You have to prise your fingers apart, but instead of a bite you find dozens of tiny pinpricks all over your palm. The tarantula's bristles were razor-sharp, and seem to have injected a stinging chemical into your skin.\n\nYou LOSE 1 Life Point.";

        Character::GAIN_LIFE(player, -1);

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

            PreText += "\n\n\"Hey, you there! What are you doing?\"\n\nYou look up to see an old peasant hurrying through the dusty orchard towards the causeway.";

            Text = PreText.c_str();
        }
        else
        {
            Text = PreText.c_str();
        }
    }
};

class Story095 : public Story::Base
{
public:
    Story095()
    {
        ID = 95;

        Text = "You trudge wearily on, but your wounds soon become infected and within a few hours you are too weak to continue. Slumping down against the trunk of a tree, you lapse into a fever while ants crawl uncaringly across your outstretched legs. Cold sweat pours off you as your limbs begin to shake, and gradually you slip into unconsciousness.\n\nTortured by the pain of fever, your mind retreats into delirium. You see your brother in the clutches of a grotesque phantom with fleshless features. Fire licks up across a sky drenched in blood, but there is no heat. The scene becomes smeared with lurid greens and violets from which skulls peer with eager watchfulness. Then, emerging from the image like a reflection in a stagnant pool, you see a colossal serpent with four leering faces...\n\nYou awaken to find yourself lying on a bed of wadded leaves. There is a smell of wood smoke and roasting meat in the air. Groaning at the ache deep in your bones, you sit up and take stock of your surroundings. You are in a hut with open walls, on the edge of a clearing. Outside you see a woodland pool surrounded by crude plots of tomato, manioc, peppers and sweet potatoes.\n\n\"So you recovered. We expected you to die.\"\n\nThe accent is lilting and unfamiliar. Craning your neck, you see a group of men in plain white robes clustered around a fire. They have your possessions spread out among them. Then you see what is roasting on the fire, and the sight makes you gasp in horror. It is a tiny baby, hideously charred as the flames lick around its thin body!";

        Image = "images/filler1.png";

        Choices.clear();
        Choices.push_back(Choice::Base("Leap up at once and rush off into the woods abandoning your belongings", 50, Choice::Type::LOSE_ALL));
        Choices.push_back(Choice::Base("Attack these cannibals at once", 73));
        Choices.push_back(Choice::Base("Wait to see what they have to say", 27));

        Controls = StandardControls();
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

class Story097 : public Story::Base
{
public:
    Story097()
    {
        ID = 97;

        Text = "The oarsmen are demons with squinting eyes and shrunken toothless gums. Stingray spines hang from their brows and upper lips where a mortal might have hair, and their flesh is a sickly blue-white colour. One wears a headdress in the shape of a shark's fin, the other has a jaguar-pelt skullcap. As the canoe draws nearer, you see they have no lower limbs: their torsos end in shapeless blobs.\n\n\"You wish to be conveyed to the Deathlands,\" says the shark paddler.\n\n\"We will take you there,\" add the jaguar paddler.\n\nLoathsome as these creatures are, you see no alternative. You climb into the boat and wait as they row through the wanly lit gloom of the cavern. Ahead lies a tunnel, but before you reach it the canoe glides to a halt beside a shelf of rock. You look up to see a narrow crevice in the wall of the cavern. It looks far from inviting, and you detect a gust of noxious air wafting out of the darkened interior.";

        Image = "images/filler1.png";

        Choices.clear();
        Choices.push_back(Choice::Base("Disembark and climb up to the crevice", 74));
        Choices.push_back(Choice::Base("Wait for the strange demons to row on", 258));

        Controls = StandardControls();
    }
};

class Story098 : public Story::Base
{
public:
    std::string PreText = "";
    Story098()
    {
        ID = 98;

        Image = "images/filler1.png";

        Choices.clear();
        Choices.push_back(Choice::Base("Go straight on", 121));
        Choices.push_back(Choice::Base("Bear left from here", 412));
        Choices.push_back(Choice::Base("Bear right", 29));

        Controls = StandardControls();
    }

    void Event(Character::Base &player)
    {
        PreText = "A cacophony of chitterings and gleeful screeches makes you look up. From the foliage overhead, dozens of beady pairs of eyes stare back at you. You laugh as you see the tiny comical faces of a troop of monkeys, teeth bared like grimacing old men.\n\nSuddenly you feel small fingers probing at your clothes. A couple of monkeys have crept up on you while the others distracted your attention. One leaps onto your head and puts its hands over your eyes while the other rifles through your belongings. You give vent to a loud curse and lunge to grab the little thieves, but they are too quick for you. You can only stand and watch helplessly as they go swinging happily off the through the trees.";

        ToLose.clear();

        Limit = 0;

        if (Character::VERIFY_ITEMS(player, {Item::Type::MAGIC_AMULET}))
        {
            ToLose.push_back({Item::Type::MAGIC_AMULET});
        }

        if (Character::VERIFY_ITEMS(player, {Item::Type::SHAWL}))
        {
            ToLose.push_back({Item::Type::SHAWL});
        }

        if (Character::VERIFY_ITEMS(player, {Item::Type::JADE_BEAD}))
        {
            ToLose.push_back({Item::Type::JADE_BEAD});
        }

        if (Character::VERIFY_ITEMS(player, {Item::Type::MAGIC_DRINK}))
        {
            ToLose.push_back({Item::Type::MAGIC_DRINK});
        }

        if (Character::VERIFY_ITEMS(player, {Item::Type::MAIZE_CAKES}))
        {
            ToLose.push_back({Item::Type::MAIZE_CAKES});
        }

        if (ToLose.size() > 1)
        {
            PreText += "\n\nThe monkeys managed to filch one of your items. Select an item to drop.";

            Limit = ToLose.size() - 1;
        }
        else if (ToLose.size() == 1)
        {
            PreText += "\n\nThe monkeys managed to filch the " + std::string(Item::Descriptions[ToLose[0]]) + ".";

            Character::LOSE_ITEMS(player, {ToLose[0]});
        }
        else
        {
            PreText += "\n\nThe monkeys did not manage to filch anything.";
        }

        Text = PreText.c_str();
    }
};

class Story099 : public Story::Base
{
public:
    Story099()
    {
        ID = 99;

        Text = "You can hear the sound of rapids up ahead, and the current carries the boat faster and faster towards them. Hurriedly tying the ROPE into a loop, you cast it towards the side of the tunnel and manage to lasso an outcrop of rock. The boat is jerked to a halt and sent drifting towards a side tunnel where the current is not so strong. You are unable to dislodge the ROPE but at least you are safe.\n\nA flicker of daylight shows at the end of the tunnel. You can smell the reek of stagnant marshland in the air. Paddling onwards, you come out into the open under an overcast sky the colour of dead skin. The river here is no more than a muddy trickle winding through sickly grey marshland. A dreary landscape of sour white clay and colourless rushes stretches far off into the distance.\n\nYou put in a rotting wooden jetty and tether the boat.";

        Image = "images/filler1.png";

        Choices.clear();
        Choices.push_back(Choice::Base("Keep the PADDLE", 53, Choice::Type::GET_ITEM, Item::Type::PADDLE));
        Choices.push_back(Choice::Base("Leave it", 53));

        Controls = StandardControls();
    }

    void Event(Character::Base &player)
    {
        Character::LOSE_ITEMS(player, {Item::Type::ROPE});
    }
};

class Story100 : public Story::Base
{
public:
    Story100()
    {
        ID = 100;

        Text = "The woman dozes until the long red rays of late afternoon are drawing back across the treetops. Suddenly her arms jerk up as though on strings, seizing the pitcher and lifting it to reveal a second head is superficially humanoid, there is no mistaking it for any human face with its staring bloodshot eyes and black slit of a mouth.\n\nLong black hair uncoils like tendrils from the monstrous head. Some of the tresses are up to two metres long, and you watch in revulsion as they form into thin matted stalks which remind you of an insect's legs. These probe the ground, preparing to support the creature's weight, and then with a grisly sucking sound the head pulls itself free of the sleeping woman's neck. As soon as it sets eyes on you it gives a gurgle of ghoulish glee and comes scuttling forward on its limbs of twisted hair. It uses some of these to propel itself up level with your face, snapping at your neck with its sharp chisel-shaped teeth while other strands of hair snake out to encircle your wrists. You cannot use a weapon now even if you have one; this struggle will be fought at close quarters.";

        Image = "images/flying-head.png";

        Choices.clear();

        Controls = StandardControls();
    }

    int Continue(Character::Base &player)
    {
        if (Character::VERIFY_SKILL(player, Skill::Type::UNARMED_COMBAT))
        {
            return 126;
        }
        else
        {
            return 149;
        }
    };
};

class Story101 : public Story::Base
{
public:
    Story101()
    {
        ID = 101;

        Text = "You find a merchant who has heard of your clan and offer him one cacao to give you lodging.\n\n\"Many people are pouring into the city from the surrounding countryside. They come to take part in the festival, and all will need a place to sleep off their excesses,\" he points out in a patient attempt to get you to offer more.\n\nYou are having none of it. \"They're just peasants,\" you counter. \"What little money they have will be spent on mead, and they'll happily sleep where they drop.\"\n\nAs an added incentive you take a cacao from your money-pouch and show it to him. This clinches the bargain, and you are given a meal and a bed in his house on the outskirts of the city.\n\nYou PAID 1 cacao for the lodging.\n\nYou can rest here for the night then decide what to do the next morning.";

        Image = "images/filler1.png";

        Choices.clear();
        Choices.push_back(Choice::Base("CODEWORD: Psychoduct", 331, Codeword::Type::PSYCHODUCT));
        Choices.push_back(Choice::Base("Head overland to Ashaka", 8, Choice::Type::LIFE, 1));
        Choices.push_back(Choice::Base("Follow the river to the coast", 30, Choice::Type::LIFE, 1));
        Choices.push_back(Choice::Base("Stay for the festival", 416, Choice::Type::LIFE, 1));

        Controls = StandardControls();
    }

    void Event(Character::Base &player)
    {
        Character::GAIN_MONEY(player, -1);
    }
};

class Story102 : public Story::Base
{
public:
    std::string PreText = "";

    Story102()
    {
        ID = 102;

        Image = "images/filler1.png";

        Choices.clear();

        Controls = StandardControls();
    }

    void Event(Character::Base &player)
    {
        PreText = "You are too proud to submit to a beating. With a great shout of rage, you charge in among them and lay about you with powerful blows. The sudden attack takes them by surprise, allowing you to badly wound several before the sheer weight of numbers begins to tell against you. For all your courage and determination, at last you are overwhelmed and pushed to the ground. Once they have you down, the guards make sure to pay you back double for every blow you struck against them.";

        auto DAMAGE = -3;

        PreText += "\n\n";

        if (Character::VERIFY_SKILL_ANY(player, Skill::Type::SWORDPLAY, {Item::Type::SWORD, Item::Type::JADE_SWORD}))
        {
            DAMAGE = -6;

            PreText += "[SWORDPLAY] ";
        }
        else if (Character::VERIFY_SKILL(player, Skill::Type::UNARMED_COMBAT))
        {
            PreText += "[UNARMED COMBAT] ";

            DAMAGE = -5;
        }

        PreText += "You LOSE " + std::to_string(-DAMAGE) + " Life Points.";

        Character::GAIN_LIFE(player, DAMAGE);

        if (player.Life > 0)
        {
            PreText += "\n\nThe guards finally tire of pummelling you. One of them spits on your swollen bloodied face, then they stalk off into the palace.";
        }

        Text = PreText.c_str();
    }

    int Continue(Character::Base &player) { return 262; }
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
        Choices.push_back(Choice::Base("Jump into the sacred well", 327));
        Choices.push_back(Choice::Base("[SPELLS] Cast a protective enchantment first", 304, Choice::Type::SKILL_ANY, Skill::Type::SPELLS, {Item::Type::MAGIC_WAND, Item::Type::JADE_SWORD}));

        Controls = StandardControls();
    }
};

class Story104 : public Story::Base
{
public:
    Story104()
    {
        ID = 104;

        Text = "The woman whose body is host to the parasitical head remains in a deep sleep, but her limbs strike out like a living puppet's. The head gnashes its teeth, screeching horribly, as it guides its stolen body forward to attack you with jerking strides.";

        Image = "images/filler1.png";

        Choices.clear();
        Choices.push_back(Choice::Base("Rush in to attack", 195));
        Choices.push_back(Choice::Base("Back away", 265));
        Choices.push_back(Choice::Base("Stand your ground and dodge to one side at the last moment", 242));

        Controls = StandardControls();
    }
};

class Story105 : public Story::Base
{
public:
    Story105()
    {
        ID = 105;

        Text = "The green-tinted stone of the staircase is almost invisible through the murky depths. You bite your lip as you consider the water of the lake. It looks almost resinous with cold. You cannot expect to survive long once you are submerged -- you would freeze to death even before you had time to run out of air.";

        Image = "images/filler1.png";

        Choices.clear();

        Controls = StandardControls();
    }

    int Continue(Character::Base &player)
    {
        if (Character::VERIFY_ITEMS(player, {Item::Type::HAUNCH_OF_VENISON}))
        {
            return 440;
        }
        else
        {
            return 150;
        }
    }
};

class Story106 : public Story::Base
{
public:
    Story106()
    {
        ID = 106;

        Text = "The nobles are discussing their route into the underworld. You are given to understand that at death most souls are conducted west across the world, entering the afterlife by means of the gate at the edge of the desert. This requires them to pass four sentinels whose duty is to prevent the living from trespassing into their realm. You catch the name of the last four sentinels whom the nobles passed on their way here, a frightful demon called Grandfather of Darkness.\n\nOne of the nobles is staring at you, and as you turn a quizzical look towards him he remarks on your resemblance to his late lord, Morning Star.\n\nExcitement quickens your blood. \"Morning Star was my brother.\"\n\nHe tells you that he was in Morning Star's retinue when it reached the palace of the wizard. He reaches behind him and produces a skull which he puts into your hands, telling you that it is your BROTHER's SKULL. Because Morning Star's soul was trapped by the wizard, he has not been able to travel on to the afterlife. This is all that remains of him.\n\nYou are filled with grief, followed by a rush of vengeful rage. You vow you will slay the wizard and free your brother's soul. Thanking the nobles for their help, you set out once again.";

        Image = "images/filler1.png";

        Choices.clear();

        Controls = StandardControls();
    }

    void Event(Character::Base &player)
    {
        Character::GET_ITEMS(player, {Item::Type::BROTHERS_SKULL});
    }

    int Continue(Character::Base &player) { return 200; }
};

class Story107 : public Story::Base
{
public:
    Story107()
    {
        ID = 107;

        Text = "Then sentinel's eyes flash with fury as he hears your words. Letting his gory maw drop open, he erupts into a long loud howl of fury that is terrifying enough to kill a weak man on the spot. Even you are flung to a cowering heap on the ground, forced to tuck your head under your arms and lie whimpering until the dreadful howling ends.\n\nAt last there is silence. You uncurl yourself and glance up towards the sentinel. Having given awful voice to his displeasure, he now shows no more interest in you. He resumes his imperious posture, sitting erect on his throne and staring directly across the passage. You might as well be a beetle for all the notice he gives you.\n\nYou slink off down the passage. You are dazed, but at first you think you have got off lightly. Then you realize how badly the sentinel's shriek addled your wits. You will need every scrap of luck to win through with just a single skill.";

        Image = "images/filler1.png";

        Choices.clear();
        Choices.push_back(Choice::Base("Select which SKILLS to lose", 37, Choice::Type::LOSE_SKILLS, 1));

        Controls = StandardControls();
    }
};

class Story108 : public Story::Base
{
public:
    Story108()
    {
        ID = 108;

        Text = "You get the servant to help you build a mound of sand roughly as long as a man's body. On top of this you sprinkle some of the dead warrior's blood, then you cover it with his jaguar-skin cloak and place his sword on top of this. At one end you carefully set his severed head. The dead eyes stare up at the stars.\n\n\"What are you going to do?\" says the servant in a frightened voice\n\n\"His spirit has but recently departed\" you reply as you take up your wand. \"I shall recall it and so rekindle the spark of life.\" So you begin what you know will be the hardest incantation of your life. For hours you continue the chant, never faltering, continually tracing occult designs in the sand around the head. The moon is dipping low across the dunes when you hoarsely utter the last syllables of the spell and slump to the ground in exhaustion.\n\nThere is a groan, but not from your lips. You look up to see the fallen warrior rising, the body beneath the cloak transformed from gore-soaked sand to living flesh and blood.\n\n\"Master,\" you hear the servant saying, \"this kind magician brought you back from the dead.\"\n\n\"Nonsense!\" scoffs the warrior. \"I must have been knocked out for a while, that's all.\"\n\nHe comes over and helps you to your feet. You have used up all your sorcery in a final miracle.\n\nYour [SPELLS] skill is lost.\n\nYou gained the codeword ANGEL.";

        Image = "images/filler1.png";

        Choices.clear();

        Controls = StandardControls();
    }

    void Event(Character::Base &player)
    {
        Character::GET_CODEWORDS(player, {Codeword::Type::ANGEL});

        Character::LOSE_SKILLS(player, {Skill::Type::SPELLS});
    }

    int Continue(Character::Base &player) { return 15; }
};

class Story109 : public Story::Base
{
public:
    Story109()
    {
        ID = 109;

        Text = "You use your illusion magic to conjure up a miniature duplicate of the tunnel. Each of the beams appears as a glowing bar tagged with a number. By touching a bar with your wand and uttering the right number, you can select and move it. This allows you to experiment without actually having to remove any of the real beams just yet. Your first few attempts all lead to the illusory tunnel collapsing, but eventually you find a combination of beams that can be removed safely.\n\nDispelling your illusion, you try removing the same arrangement of beams from the real passage. Cracks appear in the stonework and the walls sag slightly, but you are able to get through safely to the far end. The courtiers are waiting for you there.\n\n\"Magic, eh?\" says the chief courtier. \"Our master's good at magic. Better than you, perhaps.\"\n\n\"Perhaps.\"";

        Image = "images/filler1.png";

        Choices.clear();

        Controls = StandardControls();
    }

    int Continue(Character::Base &player) { return 431; }
};

class Story110 : public Story::Base
{
public:
    std::string PreText = "";

    Story110()
    {
        ID = 110;

        Image = "images/filler1.png";

        Choices.clear();

        Controls = StandardControls();
    }

    void Event(Character::Base &player)
    {
        PreText = "The knives drift in a circle around you as you move uneasily across the hall. Suddenly, one of them shoots forward and slices a gash in your arm. Recoiling in pain, you are jabbed by another. They are trying to herd you into a position where they can attack you from all directions, but you manage to duck under one as it flies in. Before they can regroup, you have run over to a corner and put back to the wall.\n\nThe ordeal continues through the night. You cannot afford to close your eyes for a moment, as the knives would then tear you to shreds. You dodge many attacks, but several cut you badly and soon your strength is ebbing along with your blood.";

        auto DAMAGE = -2;

        PreText += "\n\n";

        if (Character::VERIFY_SKILL(player, Skill::Type::UNARMED_COMBAT))
        {
            PreText += "[AGILITY] ";

            DAMAGE = -1;
        }

        PreText += "You LOSE " + std::to_string(-DAMAGE) + " Life Points.";

        Character::GAIN_LIFE(player, DAMAGE);

        if (player.Life > 0)
        {
            PreText += "\n\nAt dawn, the knives suddenly fall lifeless to the floor and soon after the courtiers come to let you out.";
        }

        Text = PreText.c_str();
    }

    int Continue(Character::Base &player) { return 132; }
};

class Story111 : public Story::Base
{
public:
    std::string PreText = "";

    Story111()
    {
        ID = 111;

        Image = "images/filler1.png";

        Choices.clear();

        Controls = StandardControls();
    }

    void Event(Character::Base &player)
    {
        PreText = "You slam into him. For a creature formed of living shadow, he feels very solid.\n\nYou LOSE 1 Life Point from the jarring impact.";

        Character::GAIN_LIFE(player, -1);

        if (player.Life > 0)
        {
            PreText += "\n\n";

            if (Character::VERIFY_SKILL(player, Skill::Type::UNARMED_COMBAT))
            {
                PreText += "[UNARMED COMBAT] You get possession of the ball and send it bouncing against the end zone, scoring a point.";

                Character::SCORE(player, player.Ticks, 1);
            }
            else
            {
                PreText += "Your opponent gets the ball and scores a point";

                Character::SCORE(player, player.Cross, 1);
            }
        }

        Text = PreText.c_str();
    }

    int Continue(Character::Base &player) { return 226; }
};

class Story112 : public Story::Base
{
public:
    Story112()
    {
        ID = 112;

        Text = "You slam the ball against the side wall then run backwards into the middle of the arena, keeping your eye on it as it bounces. A blow with your wrist sends it spinning up to strike the low-score zone, giving your team another point. It ricochets towards your opponents, who eagerly seize possession.";

        Image = "images/filler1.png";

        Choices.clear();

        Controls = StandardControls();
    }

    void Event(Character::Base &player)
    {
        Character::SCORE(player, player.Ticks, 1);
    }

    int Continue(Character::Base &player) { return 181; }
};

class Story113 : public Story::Base
{
public:
    Story113()
    {
        ID = 113;

        Text = "\"You have done me a service, and yet you ask no favour in return,\" rumbles the giant. \"Hence I shall bestow my gift: immortality.\"\n\nYou wait but nothing happens. You don't feel any different. You raise your hands; you still look the same. \"Is that it?\"\n\n\"Yes. Now your natural lifespan is infinite.\"\n\n\"My natural lifespan?\" you say. A point like this is worth getting exactly right.\n\n\"You will never die a natural death,\" the giant clarifies.\n\nYou don't know what to say. \"Er... well, thank you.\" Uppermost in your thoughts is that adventurers rarely die natural deaths in any case.\n\n\"Also, you cannot suffer gradual injury,\" adds the giant. \"A single fatal accident can kill you outright, but that is all.\"\n\nThat sounds better. As the giant said, the only thing that can now kill you is an overwhelming catastrophe like falling into a volcano.\n\nYou have become immortal. You cannot lose Life Points.";

        Image = "images/giant.png";

        Choices.clear();

        Controls = StandardControls();
    }

    void Event(Character::Base &player)
    {
        player.IsImmortal = true;
    }

    int Continue(Character::Base &player) { return 135; }
};

class Story114 : public Story::Base
{
public:
    Story114()
    {
        ID = 114;

        Text = "Midnight Bloom agrees to a detour since it will give her the chance to buy some of the fine pottery that is brought from Nachan through the fens. Putting into a lagoon where there is a small fishing village, she tells you to be quick about checking the wizard's story. \"I would like to resume our journey to Tahil at first light,\" she says.\n\nIt is already late afternoon. The sun is trawling in the red net.";

        Image = "images/filler1.png";

        Choices.clear();

        Controls = StandardControls();
    }

    int Continue(Character::Base &player) { return 260; }
};

class Story115 : public Story::Base
{
public:
    std::string PreText = "";

    Story115()
    {
        ID = 115;

        Image = "images/filler1.png";

        Choices.clear();
        Choices.push_back(Choice::Base(Skill::SPELLS.Name, 92, Choice::Type::SKILL_ANY, Skill::Type::SPELLS, {Item::Type::MAGIC_WAND, Item::Type::JADE_SWORD}));
        Choices.push_back(Choice::Base("Flee for your life", 137));

        Controls = StandardControls();
    }

    void Event(Character::Base &player)
    {
        PreText = "The whirlwinds strike you with frightening force. The air is sucked out of your lungs and your limbs are stretched until you fear they will be torn from their sockets. With a burst of desperate strength, you break free of the demon's clutches and go staggering back across the sand.";

        auto DAMAGE = -3;

        PreText += "\n\n";

        if (Character::VERIFY_SKILL_ANY(player, Skill::Type::SWORDPLAY, {Item::Type::SWORD, Item::Type::JADE_SWORD}))
        {
            DAMAGE = -1;

            PreText += "[SWORDPLAY] ";
        }
        else if (Character::VERIFY_SKILL(player, Skill::Type::UNARMED_COMBAT))
        {
            PreText += "[UNARMED COMBAT] ";

            DAMAGE = -2;
        }

        PreText += "You LOSE " + std::to_string(-DAMAGE) + " Life Points.";

        Character::GAIN_LIFE(player, DAMAGE);

        if (player.Life > 0)
        {
            PreText += "\n\nYou must try another tack.";
        }

        Text = PreText.c_str();
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
        Choices.push_back(Choice::Base("Visit the temple of the War God", 231));
        Choices.push_back(Choice::Base("Vist the temple of the Moon Goddess", 254));
        Choices.push_back(Choice::Base("Visit the temple of the Death God", 277));
        Choices.push_back(Choice::Base("You do not think any of the priests will be of much help,and would ask the Matriarch to let you have some of the clan treasures", 138));

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

        Bye = "Bidding the peasant good.day, you set off once more towards Yashuna";

        Shop = {{Item::Type::SERPENT_BRACELET, 1}};

        Controls = ShopControls();
    }

    int Continue(Character::Base &player) { return 163; }
};

class Story118 : public Story::Base
{
public:
    Story118()
    {
        ID = 118;

        Text = "Straying deeper in the forest, you surprise a deer which bolts off through the undergrowth. It reminds you how hungry you are now.";

        Image = "images/filler1.png";

        Choices.clear();
        Choices.push_back(Choice::Base("[TARGETING] Hunt for food", 187, Skill::Type::TARGETING));
        Choices.push_back(Choice::Base("[WILDERNESS LORE] Set a trap ", 210, Skill::Type::WILDERNESS_LORE));
        Choices.push_back(Choice::Base("[ROGUERY] Sneak up on some game", 233, Skill::Type::ROGUERY));
        Choices.push_back(Choice::Base("Search among your belongings for something to eat", 256));

        Controls = StandardControls();
    }
};

class Story119 : public Story::Base
{
public:
    Story119()
    {
        ID = 119;

        Text = "Thoughts gradually come trickling back -- and, along with them, a dull sense of pain. You realize you are alive. A cold wet rock presses against your back. Above you is not the open sky but a grey-lit cavern roof.\n\nYou slowly raise your head. Your ordeal has left you as weak as a newborn child, and each movement feels as though the entire weight of the earth were pressing down on your limbs. You see that the slab of smooth rock on which you are lying is surrounded by an underground lake. Blood flows freely from a deep gash in your thigh, streaming into the black water like sunset swallowed up by the night.\n\nA great reptilian head floats alongside the rock. You have the impression of an immense body of glistening green coils. Eyes which are aglow with the lore of centuries stare back at yours. The serpent's tongue flickers out on the water, tasting the blood that swirls there.\n\nYou slump back wearily. You are lying in the underworld and a water serpent is feasting on your lifeblood.";

        Image = "images/filler1.png";

        Choices.clear();
        Choices.push_back(Choice::Base("Struggle to your feet and chase the serpent off", 166));
        Choices.push_back(Choice::Base("Lie still and do nothing", 143));

        Controls = StandardControls();
    }
};

class Story120 : public Story::Base
{
public:
    Story120()
    {
        ID = 120;

        Text = "Following the crowds, you pass under an archway at the north edge of the city. People go milling past and you find yourself carried by the surge of bodies along a plaster-paved causeway that leads through light woodland. You are surprised to see no buildings on this side of the city. The crowds are all eagerly discussing some great spectacle that lies in store, and you catch snatches of conversation as you are borne along. \"Now the Rain God will no longer forsake us,\" says one man. A woman who is carrying two squalling brats shouts back over her shoulder: \"Just so long as the sacrifice pleases him! Don't forget that.\"\n\nThen the trees give way to an immense clearing. At first you cannot tell what lies ahead, but by pushing your way forward you reach the front of the crowd. Your breath escapes from your lungs in a gasp of awe. The clearing is formed by a gigantic hole in the ground. It looks as though the crust of the earth simply crumbled away to real an entrance into the underworld. The sinkhole is more than twenty metres deep and even a strong man could never hope to cast a spear right across to the far side. The sides of the pit are raw limestone clothed in a dry tangled mass of roots and creepers, dropping right down to the murky lake that occupies the bottom of this vast cavernous gulf.\n\n\"What is it?\" you ask a priest standing beside you.\n\nWhen he answers, you discover that your first wild impression was correct. \"This is the sacred well of Yashuna,\" he replies in a stately voice. \"It is the mouth of the underworld.\"";

        Image = "images/cenote.png";

        Choices.clear();

        Controls = StandardControls();
    }

    int Continue(Character::Base &player)
    {
        if (Character::VERIFY_ITEMS(player, {Item::Type::SERPENT_BRACELET}))
        {
            return 257;
        }
        else
        {
            return 234;
        }
    }
};

class Story121 : public Story::Base
{
public:
    std::string PreText = "";

    Story121()
    {
        ID = 121;

        Image = "images/filler1.png";

        Choices.clear();
        Choices.push_back(Choice::Base("Go right", 75));
        Choices.push_back(Choice::Base("Go left", 412));
        Choices.push_back(Choice::Base("Go straight ahead", 129));

        Controls = StandardControls();
    }

    void Event(Character::Base &player)
    {
        PreText = "You stumble into a bush whose sharply hooked leaves give you some nasty scratches. In ordinary circumstances such wounds would only be a painful nuisance. Here in the feverish dankness of the jungle, they soon go septic and begin to weep.\n\nYou LOSE 1 Life Point.";

        Character::GAIN_LIFE(player, -1);

        if (player.Life > 0)
        {
            PreText += "\n\nYou must decide what route to follow from here.";
        }

        Text = PreText.c_str();
    }
};

class Story122 : public Story::Base
{
public:
    Story122()
    {
        ID = 122;

        Text = "You lash out with lightning speed, pinning the snake's head against the cliff-face before it can dodge. It writhes, hissing angrily and slapping the stone with its muscular coils, but is powerless to break free. You apply increasing pressure to its neck until it goes limp and drops to fall with a heavy plop in the river below.\n\nYou peer into the tomb. The darkness seems to rustle with unseen threats, but you know that it is just a figment of your imagination. You have dealt with the tomb guardian. Now you are eager to see if there is any treasure to be had.";

        Image = "images/flying-snake.png";

        Choices.clear();

        Controls = StandardControls();
    }

    int Continue(Character::Base &player) { return 339; }
};

class Story123 : public Story::Base
{
public:
    Story123()
    {
        ID = 123;

        Text = "\"Do you know the myths of the twins called Forethought and Afterthought?\" she asks. You shake your head. \"Well, it is very popular in these parts. This picture deals with the part of the story when the twins have crossed the desert and are about to pass into the Deathlands. First they must greet each of the four sentinels correctly: Lord Skull, Lord Blood -- \"\n\n\"Milady,\" calls out one of the artisans, interrupting her. \"We've run out of the green dye. How about using blue for these feathers in the bloke's hat?\"\n\n\"No, no! That won't do!\" she cries. Turning to you, she mumbles, \"Please excuse me...\" and hurries off to remonstrate with the artisans.";

        Image = "images/filler1.png";

        Choices.clear();
        Choices.push_back(Choice::Base("Seek an audience with the King", 77));
        Choices.push_back(Choice::Base("Pay for lodging in the city if you have nay money", 101, Choice::Type::MONEY, 1));

        Controls = StandardControls();
    }
};

class Story124 : public Story::Base
{
public:
    Story124()
    {
        ID = 124;

        Text = "Luckily for you, the royal guards consider that a simple task like beating up offenders is beneath their dignity. They give you a few sharp blows to teach you a lesson, then shove you across the courtyard. \"Beat it!\" growls one, jabbing you in the kidneys with the end of his staff. \"I don't want to catch sight of you hanging around here again.\"\n\nYou LOSE 2 Life Points.";

        Image = "images/filler1.png";

        Choices.clear();

        Controls = StandardControls();
    }

    void Event(Character::Base &player)
    {
        Character::GAIN_LIFE(player, -2);
    }

    int Continue(Character::Base &player) { return 262; }
};

class Story125 : public Story::Base
{
public:
    Story125()
    {
        ID = 125;

        Text = "Scattering the chillies onto the dragon's tongue has the desired effect. He opens his mouth and spits you out with a great bellow of pain and surprise. You scrabble off to a safe distance before turning to watch his anguished attempts to wipe his tongue clean against the clifftop.\n\nKawak's rear head has blunt face with upcurving tusks and pallid globular eyes. He glowers at you and speaks with difficulty because of his burning tongue, saying, \"If you attempt to return this way, I shall devour you.\"\n\nBearing this warning in mind, you hurry onwards.";

        Image = "images/filler1.png";

        Choices.clear();

        Controls = StandardControls();
    }

    void Event(Character::Base &player)
    {
        Character::LOSE_ITEMS(player, {Item::Type::CHILLI_PEPPERS});
    }

    int Continue(Character::Base &player) { return 263; }
};

class Story126 : public Story::Base
{
public:
    Story126()
    {
        ID = 126;

        Text = "The head's spittle sprays into your eyes, stinging you like venom. Its strands of hair hug it to your neck, and you see its red eyes blazing with impending triumph. You must act quickly, or all is lost.";

        Image = "images/flying-head.png";

        Choices.clear();
        Choices.push_back(Choice::Base("Deliver a forearm smash to its jaw", 334));
        Choices.push_back(Choice::Base("Dive to the ground and try to force its face into the dirt", 311));
        Choices.push_back(Choice::Base("Attempt to batter it against the bole of the nearby tree", 378));

        Controls = StandardControls();
    }
};

class Story127 : public Story::Base
{
public:
    Story127()
    {
        ID = 127;

        Text = "You turn away and push your way out of the crowd. There are those who say it is a great honour to be chosen for sacrifice, but you have no desire to witness the death of the two young people so soon after your own bereavement.\n\nYou journey north until the causeway ends. Dusty tracks fringed with scrubland carry you the rest of the way to the coast. A farmer directs you to the village of Balak. You pass through the streets, pace quickening as you catch the enticing smell of salt spray on the air. You emerge from between two high-roofed houses and there is the sea spread out in front of you, glittering under a cloudless blue sky.";

        Image = "images/filler1.png";

        Choices.clear();
        Choices.push_back(Choice::Base("Use LETTER OF INTRODUCTION", 370, Item::Type::LETTER_OF_INTRODUCTION));
        Choices.push_back(Choice::Base(Skill::SEAFARING.Name, 391, Skill::Type::SEAFARING));
        Choices.push_back(Choice::Base("Otherwise", 158));

        Controls = StandardControls();
    }
};

class Story128 : public Story::Base
{
public:
    Story128()
    {
        ID = 128;

        Text = "You now realize that drinking from the well would have enabled you to see the occupants of the Deathlands as normal folk, but without that magic you must see them as they truly are in the eyes of the living: skeletal remains encrusted with teeming mould. Before you succumb to blind panic, you raise your wand and cast an enchantment of illusion over your own vision, allowing you to see them as they were in life. Now you can converse with them without a feeling of terror.";

        Image = "images/filler1.png";

        Choices.clear();

        Controls = StandardControls();
    }

    int Continue(Character::Base &player) { return 106; }
};

class Story129 : public Story::Base
{
public:
    Story129()
    {
        ID = 129;

        Text = "He gives a wild shriek of rage and plunges both knives towards you. The attack comes with such speed that you have no time to react, and for a split-second it seems your time has come. But the sentinel stops his lunge with perfect precision so that the tips of the blades just prick your skin. You look down to see two bright drops of blood forming on your chest.\n\nYou PERMANENTLY LOSE 2 Life Points. This is no ordinary wound, however. No one can recover from an injury dealt by Lord Blood's knives.";

        Image = "images/filler1.png";

        Choices.clear();

        Controls = StandardControls();
    }

    void Event(Character::Base &player)
    {
        player.MAX_LIFE_LIMIT -= 2;

        Character::GAIN_LIFE(player, -2);
    }

    int Continue(Character::Base &player) { return 84; }
};

class Story130 : public Story::Base
{
public:
    Story130()
    {
        ID = 130;

        Text = "The symbol on the diadem represents the World Tree, the source of all birth and regeneration. Whether its power is enough to restore the dead to life remains to be tested.\n\nYou carefully set the severed head upright on the sand and place the diadem over its brow. \"What do we do now?\" breathes the servant.\n\nYou admit you don't know. \"Just wait, I suppose.\"\n\nThe shadows flow like ink as the moon rises higher, laying a cool white patina over the desert that lends it a sense of strange enchantment. If you are ever to witness a miracle, tonight would be the time for it.\n\nThe moon reaches its zenith. Both you and the servant gaze up into the night sky, overawed by a shared sense of wonder at the countless stars. A moment later you share something else: a startled jump as a voice calls: \"By the gods! What am I doing buried up to my neck in the sand?\"\n\nAlthough you were waiting for such a miracle, it is no less amazing now that it has happened. You dig the sand out from around the warrior's head to find his body has been wholly restored by the magic of the diadem. \"A miracle!\" gasps the servant. \"Master, this kind of traveller brought you back from the dead.\"\n\n\"Nonsense,\" snaps the warrior, scuffing sand off his limbs, \"I must have been knocked out, that's all. Fancy burying me in the sand, you daft idiots!\"\n\nIt seems he intends to keep the GOLD DIADEM.\n\nYou gained the codeword ANGEL.";

        Image = "images/filler1.png";

        Choices.clear();

        Controls = StandardControls();
    }

    void Event(Character::Base &player)
    {
        Character::GET_CODEWORDS(player, {Codeword::Type::ANGEL});

        Character::LOSE_ITEMS(player, {Item::Type::GOLD_DIADEM});
    }

    int Continue(Character::Base &player) { return 15; }
};

class Story131 : public Story::Base
{
public:
    Story131()
    {
        ID = 131;

        Text = "You have held back from using the MAN OF GOLD until you really needed its power. Now the time has come. Holding the little manikin in your hand, you wait until you feel it beginning to stir and then place it on the floor of the passage.\n\nThe MAN OF GOLD strikes a haughty pose as he surveys the tangle of wooden beams. His elongated head and theatrically imperious stance remind you of the nobles of long ago whom you have seen depicted in ancient paintings. Striding forward, he takes hold of two of the beams and braces them against the side walls. His strength is incredible -- despite his tiny size, he is able to support the whole passage while you push the other beams out of the way and continue on to the far end. Once you are safe, he drops the beams he is holding and the roof caves in. Coughing at the clouds of rock dust, you peer through the rubble but can see no sign of the MAN OF GOLD.\n\nThe courtiers are already here waiting. \"You should have kept that manikin until you faced a real challenge,\" they mutter insidiously.";

        Image = "images/filler1.png";

        Choices.clear();

        Controls = StandardControls();
    }

    void Event(Character::Base &player)
    {
        Character::LOSE_ITEMS(player, {Item::Type::MAN_OF_GOLD});
    }

    int Continue(Character::Base &player) { return 431; }
};

class Story132 : public Story::Base
{
public:
    Story132()
    {
        ID = 132;

        Text = "The following evening you are shown into the House of Cold, where sparkling sheets of ice encase the walls and long icicles form pillars from floor to ceiling. Your breath curls like smoke in the freezing air as you stand shivering and watch the courtiers swing the heavy door shut.";

        Image = "images/filler1.png";

        Choices.clear();
        Choices.push_back(Choice::Base("Use a STONE and LUMP OF CHARCOAL", 155, Choice::Type::ALL_ITEMS, {Item::Type::STONE, Item::Type::LUMP_OF_CHARCOAL}));
        Choices.push_back(Choice::Base("Use a FIREBRAND", 319, Item::Type::FIREBRAND));
        Choices.push_back(Choice::Base("Otherwise", 178));

        Controls = StandardControls();
    }
};

class Story133 : public Story::Base
{
public:
    std::string PreText = "";

    Story133()
    {
        ID = 133;

        Image = "images/filler1.png";

        Choices.clear();
        Choices.push_back(Choice::Base("Run towards the enemy defensive player", 111));
        Choices.push_back(Choice::Base("Stand where you are and hope your partner can get the ball to you", 179));

        Controls = StandardControls();
    }

    void Event(Character::Base &player)
    {
        PreText = "He lunges into you as you try to get past. It is clearly a foul but from the way the courtiers are baying for blood you suspect the normal rules do not apply.";

        if (!Character::VERIFY_SKILL(player, Skill::Type::AGILITY))
        {
            Character::GAIN_LIFE(player, -1);

            PreText += "\n\nYou LOSE 1 Life Point.";
        }

        if (player.Life > 0)
        {
            if (Character::VERIFY_CODEWORD(player, Codeword::Type::SHADE))
            {
                Choices[1].Destination = 203;
            }
            else
            {
                Choices[1].Destination = 179;
            }
        }

        Text = PreText.c_str();
    }
};

class Story134 : public Story::Base
{
public:
    Story134()
    {
        ID = 134;

        Text = "\"The game is over,\" you announce, \"and we are the victors.\"\n\nThe voice speaks from the shrine in a croak of malice: \"You resorted to cheating. The wage of dishonour is death.\"\n\nA flat metallic twang builds rapidly in the dry air. There is a sour taste on your tongue, and you can feel your hair standing on end. You glance at your partner just in time to see him explode in a blossom of silent white sparks, leaving nothing but a scorched black patch on the dusty ground.\n\nNecklace of Skulls has vaporized him -- snuffed out his life with a casual flicker of sorcery! You are horrified by the callous murder, but you cannot waste time brooding on it now. If you don't act quickly, you will be next.";

        Image = "images/filler1.png";

        Choices.clear();

        Controls = StandardControls();
    }

    int Continue(Character::Base &player) { return 19; }
};

class Story135 : public Story::Base
{
public:
    Story135()
    {
        ID = 135;

        Text = "The giant gives a hiccup and something white rolls out of his mouth onto the sand.\n\n\"What's that?\" you ask him.\n\n\"Your BROTHER's SKULL,\" he replies. \"Don't thank me. Its proper place in the scheme of things is here with you. I'm just the instrument of destiny in this case.\"\n\nTucking your BROTHER's SKULL into your haversack, you set off along the beach. You have not gone more than a hundred paces when you hear a loud grunt followed by a damp sucking noise. You turn to see the giant hauling himself out of the ground. Throwing off the mass of sand and shingle that has accumulated around his body over aeons, he stands on the shore. He is big enough to climb the highest pyramid with two bounds.\n\nYou watch as the water closes over the black dome of his head, you turn away with a feeling of awe.";

        Image = "images/giant.png";

        Choices.clear();

        Controls = StandardControls();
    }

    void Event(Character::Base &player)
    {
        Character::GET_ITEMS(player, {Item::Type::BROTHERS_SKULL});

        Choices.clear();

        if (!Character::VERIFY_CODEWORD(player, Codeword::Type::SAKBE))
        {
            Choices.push_back(Choice::Base("Head on to Tahil by land", 228));
            Choices.push_back(Choice::Base("Head on to Tahil by sea", 251));
        }
    }

    int Continue(Character::Base &player) { return 300; }
};

class Story136 : public Story::Base
{
public:
    Story136()
    {
        ID = 136;

        Text = "The trader is prepared to detour south just to drop you off, but then you will have to find your own way to Tahil. \"It wasn't in our original agreement,\" he reminds you. \"My business is in Tahil, not in the fens.\"";

        Image = "images/filler1.png";

        Choices.clear();
        Choices.push_back(Choice::Base("Agree to being set down on the mainland coast south of the Isle of the Iguana", 260));
        Choices.push_back(Choice::Base("You would rather sail on to Tahil", 300));

        Controls = StandardControls();
    }
};

class Story137 : public Story::Base
{
public:
    Story137()
    {
        ID = 137;

        Text = "You race back towards the clifftops with the whirling demons hot on your heels. You can hear the screeching wind as they rush across the sand. And is it just your imagination, or can you also hear another sound behind the wind -- a sound like wild laughter?\n\nYou reach the cliff. The whirlwinds are right at your back. Trapped, you dive frantically to one side, landing heavily. You try to rise, but you are too exhausted to run any further.\n\nLuckily you do not have to. The demons are unable to stop themselves, and pitch straight over the side of the cliff. You distinctly hear their cries of outrage and shock as the swirling eddies of dust and wind tumble downwards.\n\nBreathing a sight of relief, you set off again into the west.";

        Image = "images/filler1.png";

        Choices.clear();

        Controls = StandardControls();
    }

    int Continue(Character::Base &player) { return 161; }
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
        Choices.push_back(Choice::Base("Choose from the rest of the treasures", 185));

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

        Bye = "Bidding the peasant a curt good-day, you continue along the causeway towards Yashuna";

        Shop = {{Item::Type::PAPAYA, 1}};

        Controls = ShopControls();
    }

    int Continue(Character::Base &player) { return 163; }
};

class Story140 : public Story::Base
{
public:
    Story140()
    {
        ID = 140;

        Text = "You steady yourself against the rough stone walls, only to recoil with a gasp of disgust. The brief contact has left an unpleasant coating of slime on your hands. You hurriedly wipe it away on your cloak. The stench grows stronger with each step you take until it is almost unbearable. You feel sick, but you manage to reach the chamber and stoop to inspect the item that caught your eye. It is a bowl of polished stone incised with the emblem of the Creator God, who gave life to all things. You need no special sense to recognize the aura of divine magic. This can only be the fabled CHALICE OF LIFE in which the gods brewed the primordial potion that birthed the ancestors of mankind.\n\nAs you rise to return to the boat, however, a trickle of viscous slime drips from the roof of the chamber across your face. Spluttering, you look up. A shiver of horror runs through you as you see the creatures whose lair this is.\n\nThere are about a dozen of them, clinging to the walls and ceiling of tunnel like bloated pods. They are about as big as large dogs, humanoid in the upper body but with the hindquarters of giant snails. Their flesh, where it is exposed from their shells, glistens with thick mucus. There features are horribly unformed, like babies torn prematurely from their mother's womb, and they utter soft bleating cries as they close inexorable to block your escape.";

        Image = "images/snail-creatures.png";

        Choices.clear();
        Choices.push_back(Choice::Base("Use an item", 305));
        Choices.push_back(Choice::Base("Fight your way to safety", 328));

        Controls = StandardControls();

        Take = {Item::Type::CHALICE_OF_LIFE};

        Limit = 1;
    }

    void Event(Character::Base &player)
    {
        Limit = 1;
    }
};

class Story141 : public Story::Base
{
public:
    Story141()
    {
        ID = 141;

        Text = "A grip of iron closes on your arm and you are dragged bodily into the black pit inside the tree. A musky stench makes you reel as you a are pinned against a wall of moss and decaying wood. The creature's body is covered with rough scales and it begins to strangle you with remorseless strength. You can do nothing to save yourself, and your last thought is of the GOLD DIADEM clutched in your hand. You batter it against the creature in a futile struggle, bending the soft metal with no care for its value now. You will be the richest corpse in the forest.";

        Image = "images/filler1.png";

        Type = Story::Type::DOOM;

        Choices.clear();

        Controls = StandardControls();
    }
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

        Shop = {
            {Item::Type::WATERSKIN, 1},
            {Item::Type::ROPE, 2},
            {Item::Type::TERRACOTTA_EFFIGY, 3},
            {Item::Type::BLOWGUN, 3},
            {Item::Type::INCENSE, 3}};

        Controls = ShopControls();
    }

    int Continue(Character::Base &player) { return 188; }
};

class Story143 : public Story::Base
{
public:
    Story143()
    {
        ID = 143;

        Text = "As more of your blood flows away, your growing sense of weakness begins to border on hallucination. A strange warmth spreads through you, as though the rock on which you are lying were not a cold stone in the waters of the underworld, but bathed in the rays of an unseen sun. The rhythmic slap of the serpent's tongue as it licks the blood-rimmed water lulls you into a dreamy state. Your head bobs up again, but this time without apparent effort. Staring at the serpent, you begin to imagine that you can see a tall figure standing with his feet on its coils. He looks like a king in his resplendent panoply of blue jade, copper and long turquoise quetzal feathers. You can only half make him out like an image seen in a cloudy mirror, but you see enough to tell that he is not a human being. He can only be the Rain God himself.\n\nYou LOSE 1 Life Point.";

        Image = "images/filler1.png";

        Choices.clear();
        Choices.push_back(Choice::Base("Speak to the Rain God", 212));
        Choices.push_back(Choice::Base("Rise up and fight", 166));

        Controls = StandardControls();
    }

    void Event(Character::Base &player)
    {
        Character::GAIN_LIFE(player, -1);
    }
};

class Story144 : public Story::Base
{
public:
    Story144()
    {
        ID = 144;

        Text = "The forest is a bewildering maze with walls of tattered green moss and gloomy bark. Sensing eyes upon you, you spin around but there is no one there. Are you being followed, or is your mind playing tricks on you?";

        Image = "images/filler1.png";

        Choices.clear();
        Choices.push_back(Choice::Base("Veer off to the right", 52));
        Choices.push_back(Choice::Base("Go straight on from here", 121));
        Choices.push_back(Choice::Base("Head left", 6));

        Controls = StandardControls();
    }
};

class Story145 : public Story::Base
{
public:
    Story145()
    {
        ID = 145;

        Text = "The snake's head jabs forward and you feel its fangs sink into your flesh. A sensation like acid burning its way across your chest is immediately followed by a creeping numbness; panic is replaced by ghastly calm. You stare at the hooded monster coiled at your breast. It looks like a suckling demon in one of the mythological murals on temple walls. You watch the waves of muscular effort which pulse along its neck as it pumps the contents of its venom sac into your veins.\n\nYou slump to the ledge, unable to feel the cold stone against your flesh. A cloudy film moves in from the edges of your vision. The eyes of the serpent glimmer like the first stars of evening...\n\nDarkness falls.";

        Image = "images/flying-snake.png";

        Type = Story::Type::DOOM;

        Choices.clear();

        Controls = StandardControls();
    }
};

class Story146 : public Story::Base
{
public:
    Story146()
    {
        ID = 146;

        Text = "\"As you can see, the two figures in this part of the frieze are inverted,\" says the priestess. \"This is to indicate they are in the underworld.\"\n\n\"But specifically who are they?\" you enquire. \"One seems to be a nobleman -- the other his slave, perhaps?\"\n\nShe nods. \"They are shown approaching the path into the afterlife. As the old adage goes, a rich man can only reach the afterlife if taken there by a poor man. That is why many nobles arrange to have their favourite servant buried with them in their tomb. But the picture in this case also has a symbolic meaning: the 'rich' man is the sun, escorted through the underworld each night by the planet Venus.\"\n\nHer answers are very enlightening. If only you had had such lucid instruction from the priests in Koba you would have a better understanding of the ancient myths. Before you can ask her anything else, however, she is called away to inspect some details of the mural. You are left to ponder your next move.";

        Image = "images/filler1.png";

        Choices.clear();
        Choices.push_back(Choice::Base("Go to the royal palace and ask to see the King", 77));
        Choices.push_back(Choice::Base("Spend some money on arranging a place to stay", 101, Choice::Type::MONEY, 1));

        Controls = StandardControls();
    }
};

class Story147 : public Story::Base
{
public:
    Story147()
    {
        ID = 147;

        Choices.clear();

        Controls.clear();
    }

    int Background(Character::Base &player)
    {
        if (Character::VERIFY_SKILL(player, Skill::Type::AGILITY))
        {
            return 193;
        }
        else
        {
            return 216;
        }
    }
};

class Story148 : public Story::Base
{
public:
    Story148()
    {
        ID = 148;

        Text = "The monster rushes forward on its strong stumpy legs, saliva pouring from its snapping jaws. You flinch back, expecting to feel a stab of pain as it sinks its teeth into your flesh, but it abruptly stops short as thought it has run in an invisible wall. It is powerless to do more than growl and make futile threatening lunges in your direction, but you edge past cautiously all the same.";

        Image = "images/filler1.png";

        Choices.clear();

        Controls = StandardControls();
    }

    int Continue(Character::Base &player) { return 20; }
};

class Story149 : public Story::Base
{
public:
    Story149()
    {
        ID = 149;

        Text = "Your options are limited and you must choose between them quickly.";

        Image = "images/flying-head.png";

        Choices.clear();
        Choices.push_back(Choice::Base("Match your strength against the head and try to fling it away", 334));
        Choices.push_back(Choice::Base("Fall to the ground and try grappling with it there", 311));

        Controls = StandardControls();
    }
};

class Story150 : public Story::Base
{
public:
    std::string PreText = "";

    Story150()
    {
        ID = 150;

        Image = "images/filler1.png";

        Choices.clear();

        Controls = StandardControls();
    }

    void Event(Character::Base &player)
    {
        PreText = "Bitter life-sucking cold envelops you in the moment you enter the water. The shock almost stops your heart.";

        auto DAMAGE = -2;

        PreText += "\n\n";

        if (Character::VERIFY_SKILL(player, Skill::Type::WILDERNESS_LORE))
        {
            PreText += "[WILDERNESS LORE] ";

            DAMAGE += 1;
        }

        PreText += "You LOSE " + std::to_string(-DAMAGE) + " Life Points.";

        Character::GAIN_LIFE(player, DAMAGE);

        if (player.Life > 0)
        {
            if (Character::VERIFY_SKILL(player, Skill::Type::WILDERNESS_LORE))
            {
                PreText += "\n\n[WILDERNESS LORE] Your hardiness inures you to the extreme cold.";
            }

            PreText += "\n\nHalf in a swoon, you stumble weightlessly down the stairway towards a submarine glimmer of icy green light. If you stay in this water for much longer you know you are doomed.";
        }

        Text = PreText.c_str();
    }

    int Continue(Character::Base &player) { return 173; }
};

class Story151 : public Story::Base
{
public:
    Story151()
    {
        ID = 151;

        Text = "Seized by uncontrollable horror at the sight of your companion's ghastly transformation, you turn and run, not stopping until you are far from the kapok tree and its throng of gristly nobles.";

        Image = "images/filler1.png";

        Choices.clear();

        Controls = StandardControls();
    }

    int Continue(Character::Base &player) { return 200; }
};

class Story152 : public Story::Base
{
public:
    Story152()
    {
        ID = 152;

        Text = "The palace of Necklace of Skulls cannot be far from here, since it is reputed to lie at the western rim of the world. With a wry glance at the shimmering sun, you set out across the dunes.\n\nThankfully, the sun sets at last and the cool of evening comes on the breeze. By this time you are weary with heat and thirst, but you know you must press on to cover as much ground as possible. The stars emerge like a thousand gleaming pebbles seen in a stream. Moonlight soaks the sand in hues of charcoal and silver.\n\nYou reach the crest of a dune to find a dramatic scene unfolding before your eyes. Only thirty paces away, a warrior in jaguar-hide cloak stands confronting a giant serpent with four heads. The warrior's servant holds up a burning torch to give more light as his master moves forward. The torchlight looks like fresh blood along the monster's gruesome fang-rimmed jaws.";

        Image = "images/filler1.png";

        Choices.clear();
        Choices.push_back(Choice::Base("Rush in to attack the monster", 175));
        Choices.push_back(Choice::Base("Sneak off while the warrior is fighting it", 365));
        Choices.push_back(Choice::Base("Stand by and watch what happens", 198));

        Controls = StandardControls();
    }
};

class Story153 : public Story::Base
{
public:
    Story153()
    {
        ID = 153;

        Text = "\"If this is indeed CHALICE OF LIFE,\" you tell the servant, it is the very bowl in which the Lord of all Gods gave birth to mankind.\"\n\nHe looks on, appropriately wide-eyed with awe, as you lift the dead man's head and place it within the chalice. The two of you retreat to a distance of at least thirty paces, as though tacitly sharing a fear that the spot where the chalice rests is about to be struck by a thunderbolt.\n\n In fact what happens is far stranger. As the moon nears its zenith, its rays seem to become a stream of heavy vapour pouring directly down into the chalice. Soon you cannot see the head at all because the interior of the chalice is brimming with thick white mist. This rises up into a swirling column about two metres high which just hangs there above the chalice, shining with a core of moonbeams.\n\nSuddenly a breeze arises briefly. This mist disperses at one, and as the last strands are blown away you behold the warrior standing in the chalice, his body once more made whole. He opens his eyes and watches you approach. \"What are you gawping at?\" he says.\n\nThe servant is so confused by feelings of joy, amazement and superstitious fear that he falls to the ground in a near faint. \"I've just restored you to life with that chalice,\" you tell the warrior.\n\n\"Nonsense!\" He looks down. \"What chalice?\"\n\nYou note with dismay that the chalice has indeed vanished. Obviously such power is not meant to stay in one mortal's hands for long. You just hope that you used it wisely.\n\nYou gained the codeword ANGEL.";

        Image = "images/filler1.png";

        Choices.clear();

        Controls = StandardControls();
    }

    void Event(Character::Base &player)
    {
        Character::GET_CODEWORDS(player, {Codeword::Type::ANGEL});

        Character::LOSE_ITEMS(player, {Item::Type::CHALICE_OF_LIFE});
    }

    int Continue(Character::Base &player) { return 15; }
};

class Story154 : public Story::Base
{
public:
    std::string PreText = "";

    Story154()
    {
        ID = 154;

        Image = "images/filler1.png";

        Choices.clear();

        Controls = StandardControls();
    }

    void Event(Character::Base &player)
    {
        PreText = "You remove a second beam without mishap -- and a third. Just as you are beginning to feel confident, a deep crack appears instantaneously across the roof of the passage. It is accompanied by a growl of breaking rock. Your body stiffens. You are on the point of jumping back, but it is too late. A cascade of rubble knocks you flat. You lie with your hands over your head as rocks batter you, bruising ad scraping your flesh. You wince as you feel a rib cracking.";

        Character::GAIN_LIFE(player, -3);

        if (player.Life > 0)
        {
            PreText += "\n\nYou crawl on the end of the passage where the dog-like courtiers await you. \"I had a lucky escape,\" you say as you stagger to your feet.\n\nThe chief courtier sniggers. \"That was just a little practical joke of ours,\" he says. \"I thought you'd have no trouble! You'll have to buck your ideas up if you're going to survive the real tests.\"";
        }
        else
        {
            PreText += "You were buried underneath the rubble.";
        }

        Text = PreText.c_str();
    }

    int Continue(Character::Base &player) { return 431; }
};

class Story155 : public Story::Base
{
public:
    Story155()
    {
        ID = 155;

        Text = "Unravelling a few fibres from your clothes, you use the stone to strike sparks off the walls until you have set the fibres alight. Then you carefully ignite a few splinters of charcoal and use this to get the rest of the lump burning.\n\nThe charcoal gives scant warmth, but it is better than nothing. Huddling beside your tiny fire, you spend a long miserable night waiting for the courtiers to let you out of the House of Cold.";

        Image = "images/filler1.png";

        Choices.clear();

        Controls = StandardControls();
    }

    void Event(Character::Base &player)
    {
        Character::LOSE_ITEMS(player, {Item::Type::LUMP_OF_CHARCOAL});
    }

    int Continue(Character::Base &player) { return 202; }
};

class Story156 : public Story::Base
{
public:
    std::string PreText = "";

    Story156()
    {
        ID = 156;

        Image = "images/filler1.png";

        Choices.clear();

        Controls = StandardControls();
    }

    void Event(Character::Base &player)
    {
        if (Character::VERIFY_CODEWORD(player, Codeword::Type::POKTAPOK))
        {
            PreText = "You manage to send the ball soaring to strike one of the zones marked out along the top of the wall. You score a point.";

            Character::SCORE(player, player.Ticks, 1);
        }
        else
        {
            PreText += "You stumble and jar your elbow on the side pall, recoiling in agony while your opponent gains possession of the ball.";
        }

        Text = PreText.c_str();
    }

    int Continue(Character::Base &player)
    {
        if (Character::VERIFY_CODEWORD(player, Codeword::Type::SHADE))
        {
            return 203;
        }
        else
        {
            return 179;
        }
    }
};

class Story157 : public Story::Base
{
public:
    Story157()
    {
        ID = 157;

        Text = "You do not need to fight Necklace of Skulls. You have beaten him at every turn, and he knows it. \"Listen to me, wizard,\" you call up to him. \"You put dangers across my path, but I prevailed. Your courtiers tested me with their ordeals, but I survived. You conjured men of shadow to contend against me in the ball game, but I won. You slew my brother once, but here he stands beside me, alive again!\"\n\nThere is no reply. Only cold brooding silence emanates from the lightless depths of the shrine.\n\nYou raise your fist. \"Stay out of our way in the future, Necklace of Skulls,\" you warn him. \"Keep to your palace and don't trouble living men with your noxious ways. Otherwise, my brother and I will return and pull that shrine down on top of you.\"\n\nThere is a long pause like the sigh of breath in a dying man's throat. \"Very well,\" says the sorcerer's voice. \"I agree to those terms.\"\n\nThe silence from the shrine somehow deepens. You sense that Necklace of Skulls has withdrawn his presence deep into the underworld.";

        Image = "images/filler1.png";

        Choices.clear();

        Controls = StandardControls();
    }

    int Continue(Character::Base &player) { return 442; }
};

class Story158 : public Story::Base
{
public:
    Story158()
    {
        ID = 158;

        Text = "You strike up conversation with a couple of traders who are down on the shore making repairs to their vessels. The first tells you he is setting out for Tahil tomorrow, and would be pleased to take you along. \"Not only for the sake of civilized company, either,\" he adds. There are pirates in the area, and an extra hand would be useful in deterring them.\"\n\n\"I have a better deterrent in mind,\" says the other man, looking up from his work. \"I shall delay my journey for a week or so. By that time, the pirates will have already made enough from raiding other vessels to sail back to their homeland, leaving the coast clear.\"\n\n\"Perhaps I could travel with you?\" you ask him.\n\nHe snorts. \"Not for free! I am partially deaf, so company is of no interest whether it be civilized or not. Also, I have just explained why I won't need a guard. If you want to come along, you can pay four cacao for your passage.\"";

        Image = "images/filler1.png";

        Choices.clear();
        Choices.push_back(Choice::Base("Sail with the first man for free", 280));
        Choices.push_back(Choice::Base("Agree to pay (4 cacao) and travel in a week", 205, Choice::Type::LOSE_MONEY, 4));

        Controls = StandardControls();
    }
};

class Story159 : public Story::Base
{
public:
    Story159()
    {
        ID = 159;

        Text = "When can you reply with a well-mannered but firm objection, the lord immediately recognizes you as a fellow noble and apologizes. \"I took you for a common trader,\" he says, bowing contritely. \"I am very story.\"\n\n\"These are difficult times,\" you say. \"Are all these people refugees from the Great City?\"\n\nHe glances along the quay. \"Either from there or the outlying farmlands. The attack on the city has sent a shockwave across the civilized world. From this day, certain fundamentals we have always taken for granted are suddenly thrown into question.\"\n\n\"Such as where to get a servant capable of mixing a decent cup of spiced cocoa,\" you put in.\n\nHe laughs. \"Exactly. Well, I'd better find another ship for my family to travel in.\"\n\nYou wave your hand expansively. \"Not at all! I shan't be needing this one again, since I'm travelling on inland. I think perhaps if you gave my, er, servants here a few cacao for their trouble they could take you back to Balak.\"\n\nYour travelling companions smile and nod their thanks to you when the lord's back is turned. Without another word, you take up your pack and set out towards Shakalla.";

        Image = "images/filler1.png";

        Choices.clear();

        Controls = StandardControls();
    }

    int Continue(Character::Base &player) { return 85; }
};

class Story160 : public Story::Base
{
public:
    Story160()
    {
        ID = 160;

        Text = "You emerge at last from the forest beside a village where you get directions to the city of Nachan. You pass west along a wooded ridge overlooking flat plains that stretch northwards towards the sea. The journey takes you a day and a night, and you have to sleep under the stars. Arising early, you yawn and stretch your cramped limbs. The countryside is swathed in fog which lies in hollows like an ocean of whiteness. You stroll onwards until the path emerges from a copse of trees and you stand overlooking a marvellous sight. You arrived at Nachan just a the moment of sunrise. The palaces and temples rise from the spectral fog: man-made hilltops thrusting through cloud. Beyond them lie the suburbs of the city, where lights twinkle like fading stars under the blanket of mist.\n\nThe warmth of day burns away the fog as you make your descent from the ridge. Now you can see the scintillant colours on the walls of the ceremonial buildings -- a vivid interplay of hues which is very unlike the austere white and red of Koba's palaces.\n\nBy the time you reach the level of the streets, the mist has retreated to just a few strands hanging around the upper steps of the pyramids and veiling the tree-covered hills that form a backdrop to the city. Already there are people hurrying to and fro. Some are carrying garlands of flowers while others in fanciful costumes are carpeting the roads with fresh west leaves. \"Is there a festival?\" you ask a passer-by.\n\n\"Indeed there is,\" he replies. \"Tonight is the anniversary of the old king's death.\"";

        Image = "images/street-scene.png";

        Choices.clear();
        Choices.push_back(Choice::Base("Break your journey here", 426));
        Choices.push_back(Choice::Base("Head on overland towards Ashaka", 8));
        Choices.push_back(Choice::Base("Follow the river to the coast", 30));

        Controls = StandardControls();
    }
};

class Story161 : public Story::Base
{
public:
    Story161()
    {
        ID = 161;

        Text = "Days pass. You have lost track of how long you have been travelling across the desert. The intense sun leeches the ground of all moisture and turns the horizon to a blaze of dazzling whiteness. Dusk brings no respite, but only an icy wind that leaves you shuddering inside your thin clothes. Your tongue is as dry as burnt paper, and blisters make every every step a misery.";

        Image = "images/filler1.png";

        Choices.clear();

        Controls = StandardControls();
    }

    int Continue(Character::Base &player)
    {
        if (Character::VERIFY_ITEMS(player, {Item::Type::WATERSKIN}))
        {
            return 184;
        }
        else
        {
            return 207;
        }
    }
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

    void Event(Character::Base &player)
    {
        Character::GET_UNIQUE_ITEMS(player, {Item::Type::LETTER_OF_INTRODUCTION});
    }

    int Continue(Character::Base &player) { return 93; }
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
        Choices.push_back(Choice::Base("Stand by and watch them pick the fruit", 96));

        Controls = StandardControls();
    }
};

class Story164 : public Story::Base
{
public:
    std::string PreText = "";

    Story164()
    {
        ID = 164;

        Image = "images/filler1.png";

        Choices.clear();

        Controls = StandardControls();
    }

    void Event(Character::Base &player)
    {
        PreText = "Its talons cut your flesh with the force of obsidian blades as you strike again and again, trying desperately to fend off the attack while climbing down out of the monster's reach. Its hideous roars would make the braves warriors go faint with terror.";

        auto ARMED = false;

        auto DAMAGE = -3;

        PreText += "\n\n";

        if (Character::VERIFY_SKILL_ANY(player, Skill::Type::SWORDPLAY, {Item::Type::SWORD, Item::Type::JADE_SWORD}))
        {
            PreText += "[SWORDPLAY] ";

            DAMAGE = -1;

            ARMED = true;
        }
        else if (Character::VERIFY_SKILL(player, Skill::Type::UNARMED_COMBAT))
        {
            PreText += "[UNARMED COMBAT] ";

            ARMED = true;
        }

        PreText += "You LOSE " + std::to_string(-DAMAGE) + " Life Points.";

        Character::GAIN_LIFE(player, DAMAGE);

        if (player.Life > 0)
        {
            if (ARMED)
            {
                PreText += "\n\nYou are able to deflect the worst lunges.";
            }

            PreText += "\n\nYou reach the ground and run off into the undergrowth. The creature shrieks its rage to the forest depths, unleashing a curse which pursues you with inescapable magical force: \"Thief, you will never again pilfer so easily, for I curse you now with clumsiness. Your foot will grow clubbed like a gnarled knot of wood, and you will stumble and falter from this day until the time of your death.\"";

            if (Character::VERIFY_SKILL(player, Skill::Type::AGILITY))
            {
                if (Character::VERIFY_SKILL(player, Skill::Type::CHARMS))
                {
                    PreText += "\n\nYou are able to counteract the curse with sorcery of your own.";
                }
                else
                {
                    Character::LOSE_SKILLS(player, {Skill::Type::AGILITY});

                    PreText += "\n\nYour [AGILITY] skill is lost.";
                }
            }

            PreText += "\n\nThe stabai have made themselves scarce, affrighted by the monster's anger. You are alone. Running until you have left the dead tree far behind, you lean breathlessly against a fallen log to examine the diadem. It is inlaid with a jade plaque in the cruciform shape of the sacred Tree of Life (GOLD DIADEM). You drop it into your pack ruefully, as it cost you dear.";

            Character::GET_ITEMS(player, {Item::Type::GOLD_DIADEM});
        }

        Text = PreText.c_str();
    }

    int Continue(Character::Base &player) { return 324; }
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

    void Event(Character::Base &player)
    {
        Character::GAIN_MONEY(player, -1);
    }

    int Continue(Character::Base &player) { return 209; }
};

class Story166 : public Story::Base
{
public:
    std::string PreText = "";

    Story166()
    {
        ID = 166;

        Image = "images/filler1.png";

        Choices.clear();

        Controls = StandardControls();
    }

    void Event(Character::Base &player)
    {
        PreText = "You realize that the sense of lassitude that afflicts you is a trance brought on by magic. The spell is broken as you rise to your feet and lash out at the serpent. It rears up from the water with an angry hiss. Its body is as thick as a tree and it has fangs like scythes of sharpened glass.\n\nThe battle is brief and bloody.";

        auto DAMAGE = -4;

        PreText += "\n\n";

        if (Character::VERIFY_SKILL_ANY(player, Skill::Type::SWORDPLAY, {Item::Type::SWORD, Item::Type::JADE_SWORD}))
        {
            DAMAGE = -2;

            PreText += "[SWORDPLAY] ";
        }
        else if (Character::VERIFY_SKILL(player, Skill::Type::UNARMED_COMBAT))
        {
            PreText += "[UNARMED COMBAT] ";

            DAMAGE = -3;
        }

        PreText += "You LOSE " + std::to_string(-DAMAGE) + " Life Points.";

        Character::GAIN_LIFE(player, DAMAGE);

        if (player.Life > 0)
        {
            PreText += "\n\nYou manage to drive the serpent off at last. You watch it swim away, sliding its glistening coils beneath the black water.\n\nThe soft sound of oars echoes across the cavern. You turn to see a canoe approaching out of the gloom, paddled by two of the strangest creatures you have ever seen.";
        }

        Text = PreText.c_str();
    }

    int Continue(Character::Base &player) { return 97; }
};

class Story167 : public Story::Base
{
public:
    std::string PreText = "";

    Story167()
    {
        ID = 167;

        Text = "The two demons continue their senseless chortling as they paddle you away form the rock tombs. It is a if they share some private joke -- and you have the unpleasant feeling that the joke is at your expense.";

        Image = "images/filler1.png";

        Choices.clear();

        Controls = StandardControls();
    }

    int Continue(Character::Base &player)
    {
        if (Character::VERIFY_ITEMS(player, {Item::Type::JADE_BEAD}))
        {
            return 236;
        }
        else
        {
            return 7;
        }
    }
};

class Story168 : public Story::Base
{
public:
    std::string PreText = "";

    Story168()
    {
        ID = 168;

        Image = "images/filler1.png";

        Choices.clear();

        Controls = StandardControls();
    }

    void Event(Character::Base &player)
    {
        PreText = "A rustling in the undergrowth warns you too late that what you took to be another gnarled tree root was in fact a long brown snake. It slithers away through the carpet of dead leaves after sinking its fangs into your ankle. You clutch your head as a wave of sudden weakness rolls over you. The snake was poisonous.\n\nTaking advantage of the distraction to snatch back their shawl, the stabai go loping away into the forest. Their half-audible whoops of jubilation recede until they are just like the whisper of the breeze through the leaves.\n\nYou drop to your knees as the poison burns its way through your veins.";

        auto DAMAGE = -4;

        PreText += "\n\n";

        if (Character::VERIFY_SKILL(player, Skill::Type::WILDERNESS_LORE))
        {
            DAMAGE = -2;

            PreText += "[WILDERNESS LORE] ";
        }

        PreText += "You LOSE " + std::to_string(-DAMAGE) + " Life Points.";

        Character::GAIN_LIFE(player, DAMAGE);

        if (player.Life > 0)
        {
            if (Character::VERIFY_SKILL(player, Skill::Type::WILDERNESS_LORE))
            {
                PreText += "\n\n[WILDERNESS LORE] You bandage the wound and apply herbal remedies.";
            }

            Character::LOSE_ITEMS(player, {Item::Type::SHAWL});
        }

        Text = PreText.c_str();
    }

    int Continue(Character::Base &player) { return 324; }
};

class Story169 : public Story::Base
{
public:
    Story169()
    {
        ID = 169;

        Text = "\"As you can see, the two figures in this part of the frieze are inverted,\" says the priestess. \"This is to indicate they are in the underworld.\"\n\n\"But specifically who are they?\" you enquire. \"One seems to be a nobleman -- the other his slave, perhaps?\"\n\nShe nods. \"They are shown approaching the path into the afterlife. As the old adage goes, a rich man can only reach the afterlife if taken there by a poor man. That is why many nobles arrange to have their favourite servant buried with them in their tomb. But the picture in this case also has a symbolic meaning: the 'rich' man is the sun, escorted through the underworld each night by the planet Venus.\"\n\nHer answers are very enlightening. If only you had had such lucid instruction from the priests in Koba you would have a better understanding of the ancient myths. Before you can ask her anything else, however, she is called away to inspect some details of the mural. You are left to ponder your next move.\n\nYou gained the codeword PSYCHODUCT.";

        Image = "images/filler1.png";

        Choices.clear();
        Choices.push_back(Choice::Base("Go to the royal palace and ask to see the King", 77));
        Choices.push_back(Choice::Base("Spend some money on arranging a place to stay", 101, Choice::Type::MONEY, 1));

        Controls = StandardControls();
    }

    void Event(Character::Base &player)
    {
        Character::GET_CODEWORDS(player, {Codeword::Type::PSYCHODUCT});
    }
};

class Story170 : public Story::Base
{
public:
    Story170()
    {
        ID = 170;

        Text = "You place your BLOWGUN so that it forms a bridge to the first spire of rock. Quickly crossing, you move the BLOWGUN around to the next spire and proceed in steps right across the canyon.\n\nBy the time you are nearing the far side, you can hear alarming creaks each time you tread on the BLOWGUN. It was never intended to be used like this. Reaching the safety of firm ground at last, you pick up the BLOWGUN and inspect the damage. Your weight has bent it out of shape and split the wood, making it useless. Still, at least it got you across the dreaded Death Canyon. You glance back, shuddering now that you allow yourself to imagine the long drop down through those heavy volcanic clouds. Hopefully there should be no more obstacles as perilous as that.\n\nDiscarding the broken BLOWGUN, you continue on your journey.";

        Image = "images/filler1.png";

        Choices.clear();

        Controls = StandardControls();
    }

    void Event(Character::Base &player)
    {
        Character::LOSE_ITEMS(player, {Item::Type::BLOWGUN});
    }

    int Continue(Character::Base &player) { return 263; }
};

class Story171 : public Story::Base
{
public:
    std::string PreText = "";

    Story171()
    {
        ID = 171;

        Image = "images/filler1.png";

        Choices.clear();

        Controls = StandardControls();
    }

    void Event(Character::Base &player)
    {
        PreText = "You lower yourself off the causeway. Standing waist-deep in the foul mass of maggots and murky slime gives you a strong urge to vomit. The monster is left fairly hopping with rage, shuffling from one stumpy leg to another as it watches you wade across in the direction of the jetty. \"Cowardly mortal!\" it roars. \"Afraid to face me, eh?\"\n\nWho wouldn't be afraid, with a face like that? In comparison, the maggots are unpleasant but not dangerous. You arrive at the jetty and pull yourself out of the mire, plucking maggots off the backpack which contains your possessions.";

        if (Character::VERIFY_ITEMS(player, {Item::Type::HAUNCH_OF_VENISON}))
        {
            PreText += "\n\nThe maggots CONSUMED the HAUNCH OF VENISON.";

            Character::LOSE_ITEMS(player, {Item::Type::HAUNCH_OF_VENISON});
        }

        Text = PreText.c_str();
    }

    int Continue(Character::Base &player) { return 20; }
};

class Story172 : public Story::Base
{
public:
    Story172()
    {
        ID = 172;

        Text = "\"Nightcrawlers are disembodied heads that live in calabash trees and descend to glide through the air in the dead of night,\" the fenman tells you. \"They find their way into houses through the roof, and can sometimes be heard crunching the charcoal beside the hearth. I myself once woke after a night of disturbing dreams to find my stock of firewood had mysteriously vanished.\"\n\n\"These nightcrawlers are mischievous creatures, then,\" you reply.\n\nHe gives a snort of grim laughter. \"I prefer to think of them as steeped in evil, in view of the fact that they also smother babies.\"\n\n\"I shall be sure to keep a weather eye out for flying head,\" you assure him.\n\n\"Oh, they are more cunning than that! A nightcrawler will sometimes latch onto a human neck, sinking tendrils into the host in the manner of a strangler fig taking root in another tree. In that guise, they may use trickery and guile to entice you off the road into the swamps.\"\n\n\"Presumably the presence of two heads on a body is a sure giveaway, though?\" you say.\n\nHe shrugs as though this had never occurred to him. \"Salt is the only remedy,\" he maintains. \"Nightcrawlers are repelled by salt. Farewell to you, then.\" He strolls off towards his house and you are left to mull over his advise as you continue your journey on foot.\n\nYou gained the codeword CALABASH.";

        Image = "images/filler1.png";

        Choices.clear();

        Controls = StandardControls();
    }

    void Event(Character::Base &player)
    {
        Character::GET_CODEWORDS(player, {Codeword::Type::CALABASH});
    }

    int Continue(Character::Base &player) { return 264; }
};

class Story173 : public Story::Base
{
public:
    std::string PreText = "";

    Story173()
    {
        ID = 173;

        Image = "images/filler1.png";

        Controls = StandardControls();
    }

    void Event(Character::Base &player)
    {
        Choices.clear();

        PreText = "You reach the circle of light and plunge through, relieved to discover that you have emerged into open air. It is cold here, but nothing like the deadly cold of the lake.\n\nYou wring out your clothes and look around you. The water of the lake is suspended eerily about two metres above your head, forming a roof over this strange world that extends far off into the distance. On the horizon hangs a glaring ball of white light which sends its low rays slanting across the land. The light shines straight into your eyes giving you a headache. You raise one hand to shield yourself from the glare.\n\nNow you notice that you are standing at a crossroads.";

        if (Character::VERIFY_ITEMS(player, {Item::Type::JADE_BEAD}))
        {
            PreText += "\n\nYou remember that you were advised to keep the JADE_BEAD under your tongue until you reached a crossroads. You can take it out of your mouth, and choose to discard it altogether.";

            Choices.push_back(Choice::Base("Discard the JADE BEAD first", 173, Choice::Type::LOSE_ITEM, Item::Type::JADE_BEAD));
        }

        Choices.push_back(Choice::Base(Skill::FOLKLORE.Name, 289, Skill::Type::FOLKLORE));
        Choices.push_back(Choice::Base(Skill::SPELLS.Name, 312, Choice::Type::SKILL_ANY, Skill::Type::SPELLS, {Item::Type::MAGIC_WAND, Item::Type::JADE_SWORD}));
        Choices.push_back(Choice::Base("Take the white road", 243));
        Choices.push_back(Choice::Base("Take the red road", 196));
        Choices.push_back(Choice::Base("Take the black road", 219));
        Choices.push_back(Choice::Base("Take the yellow road", 266));

        Text = PreText.c_str();
    }
};

class Story174 : public Story::Base
{
public:
    Story174()
    {
        ID = 174;

        Text = "On a impulse you drop the JADE BEAD into the well. As it falls into the water, it emits a gleam of gold-green light for an instant and then dissolves.";

        Image = "images/filler1.png";

        Choices.clear();
        Choices.push_back(Choice::Base("Drink from the well", 197));
        Choices.push_back(Choice::Base("Leave it alone", 180));

        Controls = StandardControls();
    }

    void Event(Character::Base &player)
    {
        Character::LOSE_ITEMS(player, {Item::Type::JADE_BEAD});
    }
};

class Story175 : public Story::Base
{
public:
    Story175()
    {
        ID = 175;

        Text = "You rush down the side of the dune and past the warrior, ignoring his gasp of surprise. You are eager to have the glory of killing this creature.\n\nYou gained the codeword ANGEL.";

        Image = "images/filler1.png";

        Choices.clear();

        Controls = StandardControls();
    }

    void Event(Character::Base &player)
    {
        Character::GET_CODEWORDS(player, {Codeword::Type::ANGEL});
    }

    int Continue(Character::Base &player) { return 222; }
};

class Story176 : public Story::Base
{
public:
    Story176()
    {
        ID = 176;

        Text = "With the servant, you trek on until the stars are snuffed out by a limpid haze that heralds the approach of dawn. As the heat of the day grows stifling, you find shelter beneath a sand-blasted spar of rock where you rest until nightfall. Then you gather your belongings and set out again. The moon appears over the horizon and casts its colourless beams, lighting your way.\n\n\"Am I now to serve you?\" asks the servant -- almost the first words you've exchanged since killing the hydra.\n\n\"Consider yourself a free man,\" you tell him indifferently. \"Since we may be going to our deaths, it's only fitting that in your last days you should taste the benefits of freedom.\"\n\nHe casts a woebegone look at his sagging WATERSKIN. \"If only you'd mention that earlier, I'd have retrieved my late master's WATERSKIN. It seemed disrespectful at the time.\"";

        Image = "images/filler1.png";

        Choices.clear();

        Controls = StandardControls();
    }

    int Continue(Character::Base &player)
    {
        if (Character::VERIFY_ITEMS(player, {Item::Type::WATERSKIN}))
        {
            return 220;
        }
        else
        {
            return 199;
        }
    }
};

class Story177 : public Story::Base
{
public:
    Story177()
    {
        ID = 177;

        Text = "A prayer springs to mind that seems appropriate now. As you step cautiously forward along the passage, you touch your amulet and recite: \"Let me not suffer misfortune, nor fall among deceivers; let me not suffer hurt upon the road, nor be impeded by obstacles to back or front; let me not suffer disgrace in my journey, nor be deterred by qualms. Grant me all this, O Lord of the Skies, by your divine grace.\"\n\nYou reach the end of the passage. The courtiers are crouching there in the shade of the wall. When they see you emerge unscathed they leap up with expressions of high dudgeon. The chief courtier recovers quickly from the surprise. He puts on an ingratiating smile as he slips his arm around your shoulder. \"Well, you got through that all right,\" he says. \"There's more to you than meets the eye.\"\n\nOne of the other courtiers sticks his head into the passage to inspect the vaulting. It immediately collapses. The courtier throws up his arms and has just time to give a startled yelp before he is buried under tons of falling rubble.\n\nA cloud of dust gusts out of the passage. Once it has cleared the passage is entirely blocked. There is no sign of their unlucky courtier, except for a trickle of blood that seeps from under the debris and soaks into the dust at your feet.\n\n\"Courtiers are getting thin on the ground these days,\" you say with a mocking smile.";

        Image = "images/filler1.png";

        Choices.clear();

        Controls = StandardControls();
    }

    int Continue(Character::Base &player) { return 431; }
};

class Story178 : public Story::Base
{
public:
    std::string PreText = "";

    Story178()
    {
        ID = 178;

        Image = "images/filler1.png";

        Choices.clear();

        Controls = StandardControls();
    }

    void Event(Character::Base &player)
    {
        PreText = "You feel yourself getting drowsy as the chill seeps into your bones. If you fall asleep here you will certainly perish, so you force yourself to pace up and down the long hall. Ice crunches underfoot as you walk, and your limbs are soon blue with cold.";

        auto DAMAGE = -3;

        PreText += "\n\n";

        if (Character::VERIFY_SKILL(player, Skill::Type::WILDERNESS_LORE))
        {
            PreText += "[WILDERNESS LORE] ";

            DAMAGE = -2;
        }

        PreText += "You LOSE " + std::to_string(-DAMAGE) + " Life Points.";

        Character::GAIN_LIFE(player, DAMAGE);

        if (player.Life > 0)
        {
            if (Character::VERIFY_SKILL(player, Skill::Type::WILDERNESS_LORE))
            {
                PreText += "\n\nYour natural resilience helps you resist the cold.";
            }

            PreText += "\n\nIt can only be raw determination that sustains you. When the courtiers come to open the door, you note with satisfaction that you have only one more ordeal to face. Then you will be taken to meet the one whom you hate more than any in the world, and yet have never seen. The sorcerer Necklace of Skulls.";
        }

        Text = PreText.c_str();
    }

    int Continue(Character::Base &player) { return 202; }
};

class Story179 : public Story::Base
{
public:
    std::string PreText = "";

    Story179()
    {
        ID = 179;

        Image = "images/filler1.png";

        Choices.clear();

        Controls = StandardControls();
    }

    void Event(Character::Base &player)
    {
        PreText = "The ball bounces towards your partner, who slams it across the arena in your direction.";

        PreText += "\n\n";

        auto score = 1;

        if (Character::VERIFY_SKILL(player, Skill::Type::AGILITY))
        {
            PreText += "[AGILITY] You bat it up into a high-scoring zone.";

            score = 2;
        }
        else
        {
            PreText += "You go for a safer shot but you still get one point.";
        }

        Character::SCORE(player, player.Ticks, score);

        Text = PreText.c_str();
    }

    int Continue(Character::Base &player) { return 226; }
};

class Story180 : public Story::Base
{
public:
    Story180()
    {
        ID = 180;

        Text = "You walk on until you reach the tree. The figures seated there are of macabre appearance: living skeletons whose bones are green with algae. Roots and soil clump their joints, and you can see snakes burrowing between the bars of their ribcages One raises a grinning skull-face to greet you. As it does, a butterfly opens wings of scarlet and gold across its emerald brow. If you saw such a sight in one of the murals on a temple wall, you might be moved to admire its uncanny beauty. Faced with such a thing in stark reality, however, you find yourself jumping back in fright. You hurry past without acknowledging the skeleton's welcoming gestures.";

        Image = "images/dead-nobles.png";

        Choices.clear();

        Controls = StandardControls();
    }

    int Continue(Character::Base &player) { return 200; }
};

class Story181 : public Story::Base
{
public:
    Story181()
    {
        ID = 181;

        Text = "You walk on until you reach the tree. The figures seated there are of macabre appearance: living skeletons whose bones are green with algae. Roots and soil clump their joints, and you can see snakes burrowing between the bars of their ribcages One raises a grinning skull-face to greet you. As it does, a butterfly opens wings of scarlet and gold across its emerald brow. If you saw such a sight in one of the murals on a temple wall, you might be moved to admire its uncanny beauty. Faced with such a thing in stark reality, however, you find yourself jumping back in fright. You hurry past without acknowledging the skeleton's welcoming gestures.";

        Image = "images/filler1.png";

        Choices.clear();

        Controls = StandardControls();
    }

    void Event(Character::Base &player)
    {
        if (Character::VERIFY_CODEWORD(player, Codeword::Type::VENUS))
        {
            Character::REMOVE_CODEWORD(player, Codeword::Type::VENUS);
        }
    }

    int Continue(Character::Base &player) { return 19; }
};

class Story182 : public Story::Base
{
public:
    Story182()
    {
        ID = 182;

        Text = "It is early in the morning when you make ready to sail. The sky is a shimmering pane of jade on which the last stars sparkle like dewdrops that are swiftly burnt away when the trembling red disc of the sun lurches up from the east.\n\nAlong with the half dozen other crewmen, you push the ship out through the cool grey waves and then jump aboard. Paddles are used to move out from the shores until the sail catches the breeze. Its triangular shape puts you in mind of an elegant bird unfolding its to soar.\n\nThe day passes pleasantly as you sail on keeping the shore in sight, but towards evening a cloud looms on the horizon. It indicates a storm blowing from out at sea. \"We must put out from the coastline,\" says one of the crew as you feel the wind rising. \"Otherwise we run the risk of being blown onto the reefs.\"\n\nAs the storm rolls over you, it turns the twilight to night and blots out any sign of the shore. Rain sweeps into your face, stinging your eyes with its force. The sailors cling to the mast and mutter prayers to the gods through chattering teeth. Their prayers go unheeded: the sea lifts your vessel like a toy and flings it far out into the unknown ocean.";

        Image = "images/filler1.png";

        Choices.clear();

        Controls = StandardControls();
    }

    int Continue(Character::Base &player) { return 406; }
};

class Story183 : public Story::Base
{
public:
    Story183()
    {
        ID = 183;

        Text = "\"Well, what are you waiting for?\" snaps the lord. \"I told you to get out of the ship as my family and I will now be needing it. If you expect payment --\"\n\nYou leap out onto the quay, \"Payment? For this wretched craft?\" you cry in an incredulous tone. \"Why, it has nearly cost me my life three times since I left home!\"\n\n\"It looks sturdy enough,\" he says dubiously.\n\n\"Cursed is what it is! Cursed by a woodland imp who dwelt in the tree from which the vessel was built. And cursed am I for being foolish enough to set sail in such a vessel, for I lost my aunt and most of my belongings when it last capsized under me!\"\n\nThe Lord looks at the vessel, then at the swelling crowd of refugees. Superstitious fear is struggling with necessity in his mind. \"I'll take the risk,\" he decides.\n\n\"You're a brave man and no mistake!\" you say with an admiring sigh. \"Still, I can't let you take the ship for nothing.\"\n\n\"I'm not paying you,\" he says witheringly.\n\n\"Of course not, sir. I'll pay you, for taking it off my hands.\" You reach for your money-pouch. \"I think -- well twenty cacao would be fair, considering the trouble it's given me...\"\n\nThat convinces him. He backs away, dragging his family with him. \"Keep your money! We'll find another ship.\"\n\nBidding your travelling companions farewell, you set out towards Shakalla.";

        Image = "images/filler1.png";

        Choices.clear();

        Controls = StandardControls();
    }

    int Continue(Character::Base &player) { return 85; }
};

class Story184 : public Story::Base
{
public:
    std::string PreText = "";

    Story184()
    {
        ID = 184;

        Image = "images/filler1.png";

        Choices.clear();

        Controls = StandardControls();
    }

    void Event(Character::Base &player)
    {
        PreText = "Thirst and weariness continue to sap your strength. Your small remaining supply of water is soon used up.";

        if (Character::VERIFY_SKILL(player, Skill::Type::WILDERNESS_LORE))
        {
            PreText += "\n\n[WILDERNESS LORE] You manage to ration your supplies more effectively.";
        }
        else
        {
            PreText += "\n\nYou LOSE 2 Life Points.";

            Character::GAIN_LIFE(player, -2);
        }

        if (player.Life > 0)
        {
            if (Character::VERIFY_ITEMS(player, {Item::Type::WATERSKIN}))
            {
                Character::LOSE_ITEMS(player, {Item::Type::WATERSKIN});

                PreText += "\n\nYour WATERSKIN is now used up.";
            }
        }

        Text = PreText.c_str();
    }

    int Continue(Character::Base &player) { return 152; }
};

class Story185 : public Story::Base
{
public:
    Story185()
    {
        ID = 185;

        Text = "\"I cannot take this,\" you decide, replacing the golden statuette in the chest. \"It is too precious; the clan might one day need to use it.\"\n\n\"Well said!\" declares the Matriarch, her eyes almost vanishing in her plump face as she beams with satisfaction at your answer. \"The treasures that remain are less potent in their magic, but may also prove useful.\"\n\nThere are three other items. The first is a small mirror of dark green glass with a powerful spell inscribed around the rim. \"It can be used to see into the future,\" the Matriarch tells you. \"As for this next item --\" she holds up a sealed jar \"-- it contains a MAGIC DRINK concocted by your great-grandfather which is capable of healing grievous wounds.\"\n\n\"And what of this?\" you ask, taking out a sword of sharpened green jade adorned with tiny glyphs.\n\n\"That also belonged to your great-grandfather. It served him both as a weapon of war and as a tool of his sorcery.\"";

        Image = "images/filler1.png";

        Choices.clear();

        Controls = StandardControls();

        Take = {{Item::Type::GREEN_MIRROR, Item::Type::MAGIC_DRINK, Item::Type::JADE_SWORD}};

        Limit = 2;
    }

    void Event(Character::Base &player)
    {
        Limit = 2;
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

class Story187 : public Story::Base
{
public:
    Story187()
    {
        ID = 187;

        Text = "Your dart brings down a small brocket of deer. It is scarcely bigger than a dog, but enough to assuage your hunger and leave you with a good HAUNCH OF VENISON to consume later.\n\nYou GAIN 1 Life Point.";

        Image = "images/filler1.png";

        Choices.clear();

        Controls = StandardControls();
    }

    void Event(Character::Base &player)
    {
        Character::GET_ITEMS(player, {Item::Type::HAUNCH_OF_VENISON});

        Character::GAIN_LIFE(player, 1);
    }

    int Continue(Character::Base &player) { return 279; }
};

class Story188 : public Story::Base
{
public:
    Story188()
    {
        ID = 188;

        Text = "As you leave the market, a tall fellow emerges from the portico bordering on the temple plaza and stands surveying the marketplace. He is carrying pots that mark him as a fisherman, presumably from one of the coastal towns to the north. \"Huh!\" he mutters, half to himself. \"Is there no demand for good fish these days?\"\n\n\"Probably not when it is several days old,\" you remark, grimacing at the smell wafting from his goods.\n\nHe fixes you with a stare of outrage. \"Then buy one of my pots and take up fishing yourself!\" he cries, thrusting a LOBSTER POT towards you. \"There! Destroy my livelihood, if you wish! I will sell you this pot for only two cacao.\"\n\nAs you turn to go, he adds: \"I would give anything for a taste of decent bread. I have been on the road for two days with nothing but my own fish to sustain me -- and, as you so tersely put it, they are no longer of the best quality.\"";

        Image = "images/filler1.png";

        Choices.clear();
        Choices.push_back(Choice::Base("Trade a parcel of MAIZE CAKES", 211, Choice::Type::ITEM, Item::Type::MAIZE_CAKES));
        Choices.push_back(Choice::Base("Go north", 120));
        Choices.push_back(Choice::Base("Go south", 165));

        Shop = {{Item::Type::LOBSTER_POT, 2}};

        Controls = ShopControls();
    }
};

class Story189 : public Story::Base
{
public:
    Story189()
    {
        ID = 189;

        Text = "The sun dips across the treetops to the west, sending the hazy light of late afternoon slanting down into the well. You do not need the high priest's signal to know the moment has arrived. Steeling your nerves, you step out from the platform into empty space. The creeper-clad walls of the sinkhole go rushing by as you fall, then the water suddenly seizes you in a silent icy embrace. The impact drives the air out of your lungs and you flail wildly, carried inexorably down by the weight of your gold regalia. It would spell your death, but good luck is with you as always. One of the straps was not fastened securely, and you are able to shrug out of the encumbering regalia and strike up towards the surface.\n\nIt takes longer than you would have expected. Your lungs are bursting when you finally struggle up out of the water and gulp fresh air. You are on a rocky outcrop in the middle of a subterranean lake. There is no sign of the open sky. A wan grey light hovers in the air, no brighter than dusk.\n\nA familiar noise echoes across the cavern. It is the splash of oars. You turn to see a canoe approaching, paddle by two bizarre nonhuman creatures.";

        Image = "images/filler1.png";

        Choices.clear();

        Controls = StandardControls();
    }

    int Continue(Character::Base &player) { return 97; }
};

class Story190 : public Story::Base
{
public:
    Story190()
    {
        ID = 190;

        Text = "The hard veins of luminous crystal running through the cavern wall make fine handholds. You quickly ascend to the ledge and stand inspecting the tombs. Each is sealed by a massive slab of stone bearing an engraved image of the person buried within. At first glance they look too solid for you to have nay hope of gaining entry, but then you notice that there is one slab which already has a crack in it. Even better, you discover a HAMMER lying on the ledge. You estimate that it would be an hour's hard work to smash a way into the tomb.";

        Image = "images/filler1.png";

        Choices.clear();
        Choices.push_back(Choice::Base(Skill::ROGUERY.Name, 283, Skill::Type::ROGUERY));
        Choices.push_back(Choice::Base("Break the tomb open using the HAMMER", 329));
        Choices.push_back(Choice::Base("Use the MAN OF GOLD", 306, Item::Type::MAN_OF_GOLD));
        Choices.push_back(Choice::Base("Decide against further exploration of the tombs: return to the canoe and continue on your way", 167));

        Take = {Item::Type::HAMMER};

        Limit = 1;

        Controls = StandardControls();
    }
};

class Story191 : public Story::Base
{
public:
    Story191()
    {
        ID = 191;

        Text = "Ordinarily you might try to seize the cobra before it strikes. You have heard of hunters doing this, and your own reflexes are as sharp as any man's. But at the moment your arms are trembling, muscles turned to jelly by the exertions of the past two hours. You could not rely on getting a firm grip.\n\nThe cobra is about to attack. It puts its head back, neck bracing for the lethal strike -- an action which momentarily blinds it. It is the one chance you will get, and you do not hesitate. You take a swift step closer and thrust your head forward with all your strength. There is aloud crack as your forehead slams into the cobra and crushes it back into the hard stone of the wall.\n\nYou reel back, stunned. The cobra drops limply to the floor and writhes there weakly until you recover your wits enough to stamp on it. Nursing a swelling bump on your head, you squeeze through into the tomb.";

        Image = "images/filler1.png";

        Choices.clear();

        Controls = StandardControls();
    }

    int Continue(Character::Base &player) { return 339; }
};

class Story192 : public Story::Base
{
public:
    Story192()
    {
        ID = 192;

        Text = "Your are invited to join King Cloud Jaguar and the nobles of his court in the steam-bath adjoining the palace. This is a domed room which is entered through an aperture so low that each bather has to crawl through on hands and knees.\n\nInside are rows of stone benches, and the middle of the floor is taken up by a pit filled with pebbles which have been warmed earlier in a fire until they are red-hot. Servants brings pitchers of scented water which sizzles on contact with the hot pebbles, releasing clouds of steam that make the sweat pour from your skin. At first you can hardly stand to draw a breath, but gradually you get used to the sweltering heat and start to enjoy the cleansing feeling. An old nobleman nudges you and hands you some herbs. \"Rub these on your body,\" he grunts. \"Most invigorating!\"\n\nYou regain vitality (2 Life Points) owing to the restorative effect of the herbs and the steam-bath. You also get the chance to ask about the next day's festivities, and you are told that this is the anniversary of the old king's death. When Cloud Jaguar learns of your quest, he is very impressed by your bravery. \"The pillaging of the Great City will have dire consequences,\" he says. \"I have heard tales of demons and werewolves ransacking the temples. Perhaps you can find out the truth on the matter.\"\n\nYou bow respectfully. \"I will try, your Majesty.\"\n\n\"You will spend the night in the shrine atop my father's pyramid,\" he continues. \"A tube connects the shrine to the tomb chamber. If it is the will of the gods, my father's spirit may appear to you and offer guidance.\"\n\nYou cannot refuse without giving offence. Whatever you think of meeting the late king's ghost, you must do as Cloud Jaguar has commanded. You spend the rest of the day in a mood of excitement tinged with dread, and at nightfall you are taken up to the top of the pyramid and left alone to await the ghost's appearance.";

        Image = "images/filler1.png";

        Choices.clear();

        Controls = StandardControls();
    }

    void Event(Character::Base &player)
    {
        Character::GAIN_LIFE(player, 2);
    }

    int Continue(Character::Base &player) { return 415; }
};

class Story193 : public Story::Base
{
public:
    Story193()
    {
        ID = 193;

        Text = "With a long bound, you launch yourself from the edge of the canyon onto the first of the spires of rock. Only now do you discover that it is red hot, but you remain undaunted. Careless of the long drop that surrounds you on all sides, you fall into an easy leaping gait which carries you from one spire to the next without pause. You have reached the far side of the canyon even before the heat of the rock can start to scorch your shoes.";

        Image = "images/filler1.png";

        Choices.clear();

        Controls = StandardControls();
    }

    int Continue(Character::Base &player) { return 263; }
};

class Story194 : public Story::Base
{
public:
    std::string PreText = "";

    Story194()
    {
        ID = 194;

        Image = "images/filler1.png";

        Choices.clear();

        Controls = StandardControls();
    }

    void Event(Character::Base &player)
    {
        PreText = "The monster charges along the causeway. Its three legs are short but thick with muscle, giving it a powerful lurching gait. You almost retch at the foul animal stench of its breath as it opens its jaws to snap at you. You dodge away, trying to move around to the side before counter-attacking. Your reasoning is that its tripedal stance will make it slow to turn. Unfortunately you guessed wrong: it simply rears back onto its hind leg and whirls about to face you, ripping a hunk of flesh from your arm as you step in to strike it.";

        auto DAMAGE = -4;

        PreText += "\n\n";

        if (Character::VERIFY_SKILL_ANY(player, Skill::Type::SWORDPLAY, {Item::Type::SWORD, Item::Type::JADE_SWORD}))
        {
            DAMAGE = -2;

            PreText += "[SWORDPLAY] ";
        }
        else if (Character::VERIFY_SKILL(player, Skill::Type::UNARMED_COMBAT))
        {
            PreText += "[UNARMED COMBAT] ";

            DAMAGE = -3;
        }

        PreText += "You LOSE " + std::to_string(-DAMAGE) + " Life Points.";

        Character::GAIN_LIFE(player, DAMAGE);

        if (player.Life > 0)
        {
            PreText += "\n\nYou finally manage to fight your way past the monster and go running on towards the jetty.";
        }

        Text = PreText.c_str();
    }

    int Continue(Character::Base &player) { return 20; }
};

class Story195 : public Story::Base
{
public:
    std::string PreText = "";

    Story195()
    {
        ID = 195;

        Image = "images/filler1.png";

        Choices.clear();

        Controls = StandardControls();
    }

    void Event(Character::Base &player)
    {
        PreText = "Despite your horror of the macabre creature, you force yourself to close with it in a desperate effort to the end the fight quickly. Its black maw drops open in a triumphant hiss as it lifts its host body's limbs to grapple with you. You are alarmed by the force in its blows: the poor woman it is attached to is being forced to do its bedding with a strength beyond human endurance.";

        auto DAMAGE = -5;

        PreText += "\n\n";

        if (Character::VERIFY_SKILL_ANY(player, Skill::Type::SWORDPLAY, {Item::Type::SWORD, Item::Type::JADE_SWORD}))
        {
            DAMAGE = -2;

            PreText += "[SWORDPLAY] ";
        }
        else if (Character::VERIFY_SKILL(player, Skill::Type::UNARMED_COMBAT))
        {
            PreText += "[UNARMED COMBAT] ";

            DAMAGE = -3;
        }

        PreText += "You LOSE " + std::to_string(-DAMAGE) + " Life Points.";

        Character::GAIN_LIFE(player, DAMAGE);

        Text = PreText.c_str();
    }

    int Continue(Character::Base &player) { return 288; }
};

class Story196 : public Story::Base
{
public:
    Story196()
    {
        ID = 196;

        Text = "You start out along the road, relieved at the fact that this leaves the dazzling sun of the underworld at your back. You trudge on for hours. For more than hours. Time begins to have no meaning. It seems you are waling on sand, illuminated by a ruddy glow. Your pulse sounds like the roar of the tide. The redness becomes a deep gloomy haze. You feel you can hardly breathe. Each step weighs you down, but you struggle onward towards a blaze of light...\n\nYou awaken with a sobbing intake of breath. You are back in your clanhouse in Koba. You have returned through time and space to the start of your adventure. You have a chance to begin again, forewarned by your previous mistakes.";

        Image = "images/filler1.png";

        Choices.clear();

        Controls = StandardControls();
    }

    void Event(Character::Base &player)
    {
        for (auto i = 0; i < Character::Classes.size(); i++)
        {
            if (player.Type == Character::Classes[i].Type)
            {
                player = Character::Classes[i];

                break;
            }
        }
    }

    int Continue(Character::Base &player) { return 1; }
};

class Story197 : public Story::Base
{
public:
    std::string PreText = "";

    Story197()
    {
        ID = 197;

        Image = "images/filler1.png";

        Choices.clear();

        Controls = StandardControls();
    }

    void Event(Character::Base &player)
    {
        PreText = "The drink proves cool and invigorating. You RECOVER 2 Life Points.";

        Character::GAIN_LIFE(player, 2);

        PreText += "\n\nYou walk on until you reach the tree, passing throngs of wandering souls on your way. All have their arms raised to shield their eyes from the terrible glare of the eternally setting sun. Those under the tree, however, lounge in comfort. They are shaded from the sunlight by the dense foliage. From their rich costumes you perceive them to be the souls of nobles, who enjoy this privilege by reason of their status.";

        if (Character::VERIFY_SKILL(player, Skill::Type::ETIQUETTE))
        {
            PreText += "\n\nThe nobles recognize you as one of them and invite you to sit.";
        }
        else
        {
            PreText += "\n\nThey react haughtily, telling you to begone.";
        }

        Text = PreText.c_str();
    }

    int Continue(Character::Base &player)
    {
        if (Character::VERIFY_SKILL(player, Skill::Type::ETIQUETTE))
        {
            return 106;
        }
        else
        {
            return 200;
        }
    };
};

class Story198 : public Story::Base
{
public:
    Story198()
    {
        ID = 198;

        Text = "You see the jagged outline of a sword in the warrior's hand. He is no fool. He realizes the sword is not long enough to reach the vulnerable point where the four huge necks join the monster's body, so he begins by dodging away from its attacks. This forces it to haul its bulk closer. As it does, the warrior suddenly rushes in to the attack.\n\nAt first it seems the ploy might work -- three of the heads are extended, slavering eagerly, and he easily jumps past their guard. But from your vantage point you see that he should have made more of his pretence at retreating, since one of the heads has stayed cautiously aloft on the long stalk of its neck, watching warily like a hovering eagle. It lashes down with terrifying ferocity when the warrior is still an arm's length from making his strike. His head is severed from his body in a single snap of those long jaws.\n\nYou hurriedly descend the slope of the dune. The fallen warrior's head rolls across the sand and comes to rest at your feet. You glare up at the monster, determined not to fall victim to it in the same way. It sees you and gives vent to four predatory snarls.\n\nYou move in. You have no choice. You must slay the hydra or you cannot reach your goal.";

        Image = "images/filler1.png";

        Choices.clear();

        Controls = StandardControls();
    }

    int Continue(Character::Base &player) { return 222; }
};

class Story199 : public Story::Base
{
public:
    std::string PreText = "";

    Story199()
    {
        ID = 199;

        Image = "images/filler1.png";

        Choices.clear();

        Controls = StandardControls();
    }

    void Event(Character::Base &player)
    {
        PreText = "You are trembling with thirst and exhaustion. Your mouth feels as dry as the endless wastes surrounding you.";

        auto DAMAGE = -2;

        PreText += "\n\n";

        if (Character::VERIFY_SKILL(player, Skill::Type::WILDERNESS_LORE))
        {
            DAMAGE = -1;

            PreText += "[WILDERNESS LORE] ";
        }

        PreText += "You LOSE " + std::to_string(-DAMAGE) + " Life Points.";

        Character::GAIN_LIFE(player, DAMAGE);

        if (player.Life > 0)
        {
            if (Character::VERIFY_SKILL(player, Skill::Type::WILDERNESS_LORE))
            {
                PreText += "\n\n[WILDERNESS LORE] You have a natural hardiness that prevented further LOSS of Life Points.";
            }
        }

        Text = PreText.c_str();
    }

    int Continue(Character::Base &player) { return 220; }
};

class Story200 : public Story::Base
{
public:
    Story200()
    {
        ID = 200;

        Text = "You find a river -- not of blood this time, but of cold pus-coloured fluid -- and follow its course through pale rolling dales until you come to a massive stone arch. Peering inside, you see steps rising up a long a tunnel that goes up through the layer of water roofing the Deathlands. From far ahead comes a shaft of true daylight. You have the way back to the land of the living.\n\nYou advance up the tunnel. Soon you can see the bright sunlight ahead, and smell the clean air of the upper world. But to reach it you must run a perilous gauntlet, for now you see that the passage is guarded by four baleful sentinels who sit in alcoves along the walls. They are nearly twice as big as you, with faces of brooding nightmare and talons like jaguar's teeth. From the legends you heard in childhood, you know that the only way safely past these four is to greet each by name.";

        Image = "images/threshold-gods.png";

        Controls = StandardControls();
    }

    void Event(Character::Base &player)
    {
        Choices.clear();

        if (!Character::VERIFY_CODEWORD(player, Codeword::Type::ZAZ))
        {
            Choices.push_back(Choice::Base(Skill::FOLKLORE.Name, 267, Skill::Type::FOLKLORE));
            Choices.push_back(Choice::Base(Skill::ROGUERY.Name, 290, Skill::Type::ROGUERY));
            Choices.push_back(Choice::Base("You have none of those", 313));
        }
    }

    int Continue(Character::Base &player) { return 244; }
};

class Story201 : public Story::Base
{
public:
    Story201()
    {
        ID = 201;

        Text = "A deep crunch reverberates through the stone around you and cracks appear in the corbels of the roof with such startling suddenness that they could almost be long splashes of ink. But you know that they are not, and even as the masonry blocks start to shift you are throwing yourself into a forward somersault that carries you safely to the end of the passage. There you turn to see a thick cloud of rock dust billowing out. When the dust settles, the passage has been entirely buried under huge slabs of fallen masonry.\n\nThe courtiers join you. \"You're quick on your feet,\" says the chief courtier, patting your shoulder.\n\nYou instinctively recoil at his touch. You make no pretence at hiding your loathing as you look at him. \" get the feeling I'm going to need to be.";

        Image = "images/filler1.png";

        Choices.clear();

        Controls = StandardControls();
    }

    int Continue(Character::Base &player) { return 431; }
};

class Story202 : public Story::Base
{
public:
    Story202()
    {
        ID = 202;

        Text = "When it is time for you to be taken to the House of Gloom, the courtiers summon you with surly grunts. It is obvious from their glowering looks that they did not expect you to endure this long. As they prod you through the doorway, the chief courtier is struck by inspiration.\n\n\"You have been cheating in the ordeals,\" he says. \"Using items to help you. Give me your travelling pack.\" He takes the pack containing your belongings and places it outside the door. \"It will be returned to you tomorrow. If you survive.\" It is a dingy cobweb-strewn chamber with many shadowy recesses. The packed-earth floor rises at intervals in long low mounds. Something about the place stirs the hairs on the nape of your neck. You feel the tingle of awakening dread as you ask: \"What is the ordeal here?\"\n\nThe chief courtier places a single short candle on the floor just inside the door. \"This is the place where our ancestors are buried. See those mounds? Their graves. If you can keep the candle alight until morning, they'll leave you alone. But it goes out then their ghosts wills be sure to pay you a visit.\"\n\nThe door grates shut, leaving just the trembling flame of the candle between you and eldritch terror.";

        Image = "images/filler1.png";

        Choices.clear();

        Controls = StandardControls();
    }

    int Continue(Character::Base &player)
    {
        if (Character::VERIFY_CODEWORD(player, Codeword::Type::IGNIS))
        {
            return 248;
        }
        else if (Character::VERIFY_SKILL(player, Skill::Type::CUNNING))
        {
            return 271;
        }
        else
        {
            return 294;
        }
    }
};

class Story203 : public Story::Base
{
public:
    Story203()
    {
        ID = 203;

        Text = "The ball bounces towards your brother, but with one arm held to his brow he is not nimble enough to get possession and send it back to you. The attacking shadow man leaps up swatting the ball against one of the scoring zones.";

        Image = "images/filler1.png";

        Choices.clear();

        Controls = StandardControls();
    }

    void Event(Character::Base &player)
    {
        Character::SCORE(player, player.Cross, 1);
    }

    int Continue(Character::Base &player)
    {
        return 226;
    }
};

class Story204 : public Story::Base
{
public:
    std::string PreText = "";

    Story204()
    {
        ID = 204;

        Image = "images/filler1.png";

        Choices.clear();

        Controls = StandardControls();
    }

    void Event(Character::Base &player)
    {
        PreText = "You place a dart directly between his cobwebby eyes. It would kill any mortal man, but Necklace of Skulls has endured for a thousand years. The sorcery he used to prolong his life turned him into a creature halfway between life and death. He reels back, giving vent to a roar of anguish that sounds like the sky being ripped in two, and pulls the dart out of his bloodless flesh.\n\nYou race up the pyramid steps towards him, ducking as he sends a fountain of ultraviolet fire streaming from his fingertips. It scorches your flesh. A direct hit would have charred you to the bone. \n\nYou LOSE 1 Life Point.";

        if (player.Life > 0)
        {
            PreText += "\n\nBefore he has time for another spell, you have closed with him for the final battle.";
        }

        Text = PreText.c_str();
    }

    int Continue(Character::Base &player)
    {
        if (Character::VERIFY_CODEWORD(player, Codeword::Type::VENUS))
        {
            return 240;
        }
        else
        {
            return 296;
        }
    }
};

class Story205 : public Story::Base
{
public:
    std::string PreText = "";

    Story205()
    {
        ID = 205;

        Image = "images/filler1.png";

        Controls = StandardControls();
    }

    void Event(Character::Base &player)
    {
        PreText = "You have a week to while away before you set sail.";

        auto FISHED = false;

        if (Character::VERIFY_ANY_SKILLS(player, {Skill::Type::WILDERNESS_LORE, Skill::Type::SEAFARING}))
        {
            FISHED = true;
        }
        else if (Character::VERIFY_ITEMS(player, {Item::Type::LOBSTER_POT}))
        {
            FISHED = true;

            Character::LOSE_ITEMS(player, {Item::Type::LOBSTER_POT});
        }

        if (FISHED)
        {
            PreText += "\n\nYou fish for food in this time.";

            if (player.Life < player.MAX_LIFE_LIMIT)
            {
                player.Life = player.MAX_LIFE_LIMIT;

                PreText += " You regain Life Points lost previously.";
            }
        }
        else
        {
            PreText += "\n\nYou are reduced to begging for scraps and collecting snails along the shore. This soon results in mild food poisoning. You LOSE 1 Life Point.";

            Character::GAIN_LIFE(player, -1);
        }

        Text = PreText.c_str();
    }

    int Continue(Character::Base &player) { return 182; };
};

class Story206 : public Story::Base
{
public:
    std::string PreText = "";

    Story206()
    {
        ID = 206;

        Image = "images/filler1.png";

        Choices.clear();

        Controls = StandardControls();
    }

    void Event(Character::Base &player)
    {
        PreText = "Your sword lashes out, clattering loudly against the lord's. The crowd stares in excitement and horror as the two of you circle warily. You see the lord's wife draw her children protectively against her skirts. You lunge in close. Your opponent's sword comes up in a desperate parry that breaks splinters off its obsidian edge. He grunts as a red weal appears across his arm, but he responds with a clubbing upswing of the sword hilt that leaves you stunned.\n\nThe fight goes on, carrying you to and fro across quay. At last you score a mighty blow that slashes his hand knocking his sword into the water. He gives a snarl which is as much annoyance as pain, then pulls his family off into the crowd.\n\nYou are bleeding from several deep cuts.\n\nYou LOSE 2 Life Points.";

        Character::GAIN_LIFE(player, -2);

        if (player.Life > 0)
        {
            PreText += "\n\nYou manage to bind your wounds with strips of cloth. Then, bidding your grateful travelling companions goodbye, you set out towards Shakalla.";
        }

        Text = PreText.c_str();
    }

    int Continue(Character::Base &player) { return 85; }
};

class Story207 : public Story::Base
{
public:
    Story207()
    {
        ID = 207;

        Text = "Your head is pounding and it feels as though a rough rope has been used to scour your throat. You recognize the symptoms of dehydration. Without water, you will die.\n\nYou find three plants that might yield the moisture you need. The first is a large barrel-shaped cactus with a milky sap. The second is a clump of rough spiky leaves with a single long stalk growing up from the centre. The last is another cactus, paler in colour than the first, comprising many flattened bulbous segments with squashy fruits.";

        Image = "images/filler1.png";

        Choices.clear();
        Choices.push_back(Choice::Base(Skill::WILDERNESS_LORE.Name, 230, Skill::Type::WILDERNESS_LORE));
        Choices.push_back(Choice::Base("Get moisture from the barrel cactus", 253));
        Choices.push_back(Choice::Base("... from the spiky-leafed plant", 276));
        Choices.push_back(Choice::Base("... from the bulbous cactus", 299));
        Choices.push_back(Choice::Base("None of these plants will help you", 322));

        Controls = StandardControls();
    }
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

        if (Character::VERIFY_ITEMS(player, {Item::Type::MAGIC_DRINK}))
        {
            PreText += Item::Descriptions[Item::Type::MAGIC_DRINK];
            PreText += "\n\nIt can be used once during your adventure. It will restore 5 lost Life Points, up to the limit of your initial Life Points score.";

            selected++;
        }

        if (Character::VERIFY_ITEMS(player, {Item::Type::GREEN_MIRROR}))
        {
            if (selected > 0)
            {
                PreText += "\n\n";
            }

            PreText += Item::Descriptions[Item::Type::GREEN_MIRROR];
            PreText += "\n\nIt can be used once -- and only once -- at any point in your adventure before deciding which you will choose.";

            selected++;
        }

        if (Character::VERIFY_ITEMS(player, {Item::Type::JADE_SWORD}))
        {
            if (selected > 0)
            {
                PreText += "\n\n";
            }

            PreText += Item::Descriptions[Item::Type::JADE_SWORD];
            PreText += "\n\nIt counts as both a sword and a wand for the purposes of skill-use.";

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

class Story209 : public Story::Base
{
public:
    Story209()
    {
        ID = 209;

        Text = "A sense of awe comes over you while walking through the green gloom that lies between the soaring tree-trunks of the forest. Dragonflies flash with the colours of copper, obsidian and gemstones as they dart in and out of the scattered beams of sunlight. Monkeys chitter unseen high above your head, crashing the thick foliage aside as they tumble from branch to branch. A dust-like swirling in the shadows is in fact the flight of countless tiny gnats. There is a hot perfumed dampness here: the rich odour of the forest floor rising to mingle with the scent that trickles down from brightly hued orchids. You pass huge fanciful growths of fungi which look like unearthly stones dropped by the gods.\n\nA sparkle of bright light catches your eye. Standing some distance off, framed in the eternal twilight of the jungle like a jewel displayed on dark cloth, is a bewitching figure. She turns her face towards you and you give a gasp of surprise. Her features -- her whole body -- are suffused with a dazzling golden radiance that seems to shine from her very skin. With a musical laugh, she swirls her SHAWL up around her shoulders and starts to dance between the trees.";

        Image = "images/filler1.png";

        Choices.clear();
        Choices.push_back(Choice::Base("Pursue her", 232));
        Choices.push_back(Choice::Base("Leave her alone", 160));

        Controls = StandardControls();
    }
};

class Story210 : public Story::Base
{
public:
    Story210()
    {
        ID = 210;

        Text = "You rig a spear trap by using creepers to lash a splintered branch to a tethered sapling. Before long a deer springs the trap and is impaled by the branch. You rush forward to administer a merciful death, then set to preparing the meat. There is enough to provide you with a good meal and leave a launch of venison over.\n\nYou GAIN 1 Life Point.";

        Image = "images/filler1.png";

        Choices.clear();

        Controls = StandardControls();
    }

    void Event(Character::Base &player)
    {
        Character::GET_ITEMS(player, {Item::Type::HAUNCH_OF_VENISON});

        Character::GAIN_LIFE(player, 1);
    }

    int Continue(Character::Base &player) { return 279; }
};

class Story211 : public Story::Base
{
public:
    Story211()
    {
        ID = 211;

        Text = "You offer to sell him your maize cakes, but he protests that his wife would not be happy if he returned home with no money to show for his journey. \"On the other hand, I could give you this parcel of salt,\" he suggests, taking a bundle of oiled cloth from his backpack. \"It is worth nothing to me on the way back to Balak, but you may be able to get a good price for it.\"";

        Image = "images/filler1.png";

        Choices.clear();
        Choices.push_back(Choice::Base("Make your way north out of the city", 120));
        Choices.push_back(Choice::Base("Go south towards the forest", 165));

        Trade = std::make_pair(Item::Type::MAIZE_CAKES, Item::Type::PARCEL_OF_SALT);

        Controls = TradeControls();
    }
};

class Story212 : public Story::Base
{
public:
    Story212()
    {
        ID = 212;

        Text = "You open your mouth to speak and the JADE BEAD rolls out. It falls, bounces off the rock and disappears into the water with a tiny splash.\n\nIn the same moment, the tenebrous image of the Rain God leaps into sharp focus. You see him as clearly now as if all the sun's light were focused just where he is standing. Everything else goes plunging into darkness. Your vision is filled with the blazing presence of the divinity.\n\nHis face is far from human; you can see that now. He opens his hand in the traditional beneficent gesture of royalty throughout the ages, inviting you to speak.\n\n\"O supreme lord...\"\n\nYou falter. How can you address a god?\n\nThen you hear his voice inside your head, telling you that he knows why you have been sent. He accepts the sacrifice. Your life will buy the heavy rains needed to irrigate the crops.\n\nYou try to open your mouth to tell him more -- about your quest to find your brother, about the thirst for truth and for vengeance on the sorcerer in the western desert. But you are too drowsy. The dazzling radiance of the Rain God's aura is veiled by a wave of darkness. You relax, strangely content.\n\nIn the gloom of the underworld, a monstrous serpent contentedly laps up the last of your blood and dives beneath the water.";

        Image = "images/filler1.png";

        Type = Story::Type::DOOM;

        Choices.clear();

        Controls = StandardControls();
    }
};

class Story213 : public Story::Base
{
public:
    std::string PreText = "";

    Story213()
    {
        ID = 213;

        Image = "images/filler1.png";

        Take = {Item::Type::HAMMER};

        Limit = 1;

        Controls = StandardControls();
    }

    void Event(Character::Base &player)
    {
        PreText = "With a wave of your wand and a muttered incantation, you animate the ROPE. Guiding it like a snake charmer, you impel it to rise up to the ledge above.";

        if (Character::VERIFY_ITEMS(player, {Item::Type::JADE_BEAD}))
        {
            PreText += "Unfortunately, the JADE BEAD rolls out of your mouth while you are speaking the spell and is lost in the water.";

            Character::LOSE_ITEMS(player, {Item::Type::JADE_BEAD});
        }

        PreText += "A tug on the ROPE confirms that it is taut. You climb up to the ledge without any trouble. The doors of the tombs are massive slabs of stone, each with a bas-relief carving of the occupant. At first glance they look impregnable, but then you notice that there is one slab which has a crack running right across it. Even better, you discover a HAMMER lying on the ledge. You estimate that it would be about an hour's hard work to smash a way into the tomb.";

        Choices.clear();

        if (!Character::VERIFY_SKILL(player, Skill::Type::ROGUERY))
        {
            Choices.push_back(Choice::Base("Smash the tomb open using the HAMMER", 329));
            Choices.push_back(Choice::Base("Use the MAN OF GOLD", 306, Item::Type::MAN_OF_GOLD));
            Choices.push_back(Choice::Base("Decide against violating the tombs and return to the canoe, continuing on your way", 167));
        }

        Text = PreText.c_str();
    }

    int Continue(Character::Base &player) { return 283; }
};

class Story214 : public Story::Base
{
public:
    Story214()
    {
        ID = 214;

        Text = "You try to paddle the canoe, but the current is too strong. You are borne helplessly on to an underground waterfall and flung out as the canoe goes plunging over the brink. Something strikes your head. There is a blaze of painful light, then darkness as you go under the surface. You drift down towards the river bed, dimly aware that your life ebbing away with the thin trickle of air bubbles rising from your slack jaw.";

        Image = "images/filler1.png";

        Type = Story::Type::DOOM;

        Choices.clear();

        Controls = StandardControls();
    }
};

class Story215 : public Story::Base
{
public:
    Story215()
    {
        ID = 215;

        Text = "The ritual ball contest is of great importance to the nobility, who often wage enormous sums on the outcome. The priests value it just as highly because of its religious significance. As an expert exponent of the contest, you are greeted like an esteemed guest. A courtier bows and users you through into the palace, to the envy of those those waiting in vain to present their petitions to the King.\"";

        Image = "images/filler1.png";

        Choices.clear();

        Controls = StandardControls();
    }

    int Continue(Character::Base &player) { return 192; }
};

class Story216 : public Story::Base
{
public:
    std::string PreText = "";

    Story216()
    {
        ID = 216;

        Image = "images/filler1.png";

        Choices.clear();

        Controls = StandardControls();
    }

    void Event(Character::Base &player)
    {
        Type = Story::Type::NORMAL;

        PreText = "You spend a minute staring down into the rolling billows of gritty smoke. It ought to be a mercy that you cannot see the bottom of the canyon, but in fact the faint glare of those distant fires only evokes the worst fears of your imagination. You make several run-ups to the edge of the canyon, stopping short each time with a gasp of sudden panic. But at last, dredging up every drop of courage, you manage to force yourself to leap out towards the first spire of rock.\n\nYou misjudged your landing. For a long agonizing second you are left teetering on the brink. Then you slip, barely managing to catch hold of the spire in time to prevent yourself plunging down into the volcanic abyss. It is only when you wrap your limbs around the spire that you discover it is baking hot. You do not have the strength to pull yourself up, and the heat will soon force you to relinquish your grip.";

        if (!Character::VERIFY_CODEWORD(player, Codeword::Type::ZOTZ))
        {
            PreText += " You are left to morbidly consider your fate in the last minutes before your strength gives out.";

            Type = Story::Type::DOOM;
        }

        Text = PreText.c_str();
    }

    int Continue(Character::Base &player) { return 239; }
};

class Story217 : public Story::Base
{
public:
    Story217()
    {
        ID = 217;

        Text = "You pop a dart into your BLOWGUN and start walking towards the monster. It shuffles eagerly from side to side on its strong stumpy legs. \"Why aren't you afraid, mortal?\" it asks. \"I'm going to swallow you up -- crunch your bones, drink your juices, and spit out the skin for my maggots to enjoy!\"\n\nYou keep on walking along the causeway, apparently unperturbed.\n\n\"You haven't got a chance!\" snarls the monster, tensing its legs to spring on you.\n\n\"Your only reply is to raise the BLOWGUN and puff your dart straight into the monster's eye. It gives a howl of pain and stumbles off the causeway into the mass of maggots, while you go racing towards the jetty.";

        Image = "images/filler1.png";

        Choices.clear();

        Controls = StandardControls();
    }

    int Continue(Character::Base &player) { return 20; }
};

class Story218 : public Story::Base
{
public:
    std::string PreText = "";

    Story218()
    {
        ID = 218;

        Image = "images/filler1.png";

        Text = NULL;

        Choices.clear();

        Controls = StandardControls();
    }

    void Event(Character::Base &player)
    {
        PreText = "As evening drapes the city in long blue shadows, you take a stroll to the perimeter of the royal compound. A high white wall encloses the palace and the tomb-pyramids of the King's ancestors. At the gateway you see a group of burly warriors armed with jag-edged swords. Their lacquered shields and resplendent feather cloaks mark them out as elite soldiers of the royal guard -- too dangerous to risk a skirmish with, no matter what your skill at arms.";

        Choices.clear();

        if (Character::VERIFY_ANY_SKILLS(player, {Skill::Type::AGILITY, Skill::Type::ROGUERY, Skill::Type::SPELLS}))
        {
            Choices.push_back(Choice::Base(Skill::AGILITY.Name, 354, Skill::Type::AGILITY));
            Choices.push_back(Choice::Base(Skill::ROGUERY.Name, 374, Skill::Type::ROGUERY));
            Choices.push_back(Choice::Base(Skill::SPELLS.Name, 396, Choice::Type::SKILL_ANY, Skill::Type::SPELLS, {Item::Type::MAGIC_WAND, Item::Type::JADE_SWORD}));
        }
        else
        {
            PreText += "\n\nYou will have to give up on your plan and decide what to do the next morning.";
            Choices.push_back(Choice::Base("Go west", 8, Choice::Type::LIFE, 1));
            Choices.push_back(Choice::Base("Go north", 30, Choice::Type::LIFE, 1));
            Choices.push_back(Choice::Base("Stay here for the festival", 416, Choice::Type::LIFE, 1));
        }

        Text = PreText.c_str();
    }
};

class Story219 : public Story::Base
{
public:
    Story219()
    {
        ID = 219;

        Text = "The road takes you into a dingy region devoid of any feature except for a sheer cliff that stretches across the horizon ahead. You see someone in the distance, shining like a jewel against the drab surrounds because of his feather cloak and turquoise head-dress. Though you call out, he is too far away to hear. As you hurry along the causeway, you see him reach the cliff and disappear into one of the two cave mouths.\n\nYou are so intent on reaching the cliff that you almost fail to notice a pitted stone idol beside the road. The skeletal jaw and blackened eyes mark it as an effigy of the Death God who rules this land. The stone bowl in front of the idol is bare of offerings, so obviously the figure in the feather cloak left nothing. On the other hand, you might consider it wise to donate some money of your own to secure the god's benison.";

        Image = "images/filler1.png";

        Choices.clear();
        Choices.push_back(Choice::Base(Skill::FOLKLORE.Name, 379, Skill::Type::FOLKLORE));
        Choices.push_back(Choice::Base("Make a Donation", 259, Choice::Type::DONATE));
        Choices.push_back(Choice::Base("Ignore the idol", 259));

        Controls = StandardControls();
    }

    void Event(Character::Base &player)
    {
        player.DONATION = 0;
    }
};

class Story220 : public Story::Base
{
public:
    std::string PreText = "";

    Story220()
    {
        ID = 220;

        Text = "The sun rises again, flooding the sands with the stifling heat of day. You realize that soon you must find shade, or the sun will bake you alive. Stumbling wearily up to the top of the next dune, however, all such thoughts fly from your mind to be replaced by a feeling of exhilaration. You have found it! The wizard's palace lies just ahead across a stretch of brown-gold sand. The dawn light makes it seem to shimmer like a mirage in the deep blue shadows between the dunes, but you know it is real.\n\nDouble doors swing open in the wall as you approach. Confronting you are a horde of men in ragged animal skins. Their long thin faces and downcast smiles give them a canine appearance. All of them bear stone axes which they lift when you walk through the palace gates -- not a gesture of immediate attack, but just to warn you where you stand.\n\n\"I have come,\" you say, \"to speak to Necklace of Skulls.\"\n\nOne of the men gives a bark of laughter. \"It is not as easy as that. Do you think our master sees every stray mongrel who wanders to his door? First you will have to pass five nights among us, his faithful courtiers.\"\n\nYou decide to change tack. \"What of my brother, Morning Star?\" you ask.\n\n\"He's been here. Perhaps you'll get to meet him -- later.\"";

        Image = "images/courtiers.png";

        Choices.clear();

        Controls = StandardControls();
    }

    int Continue(Character::Base &player)
    {
        if (Character::VERIFY_CODEWORD(player, Codeword::Type::ANGEL))
        {
            return 269;
        }
        else
        {
            return 292;
        }
    }
};

class Story221 : public Story::Base
{
public:
    Story221()
    {
        ID = 221;

        Text = "The noble gives a cry of alarm as he watches you dive off the ledge into the river of blood. Though the current is strong, you are a good swimmer and you have the added advantage of a determined heart. Your powerful strokes carry you safely through the charnel foam to the far bank, where you wring your clothes out with a grimace at how they will smell once the blood dries.";

        Image = "images/filler1.png";

        Choices.clear();

        Controls = StandardControls();
    }

    int Continue(Character::Base &player) { return 36; }
};

class Story222 : public Story::Base
{
public:
    Story222()
    {
        ID = 222;

        Text = "The hydra rolls its massive coils across the sand towards you. It moves slowly, but its heads can strike out with lightning speed. You will need to keep your wits about you if you are to survive this battle.";

        Image = "images/filler1.png";

        Choices.clear();
        Choices.push_back(Choice::Base("Try dodging away from its attack", 245));
        Choices.push_back(Choice::Base("Rush straight in towards it", 268));
        Choices.push_back(Choice::Base("Stand your ground and make ready to parry", 291));

        Controls = StandardControls();
    }
};

class Story223 : public Story::Base
{
public:
    Story223()
    {
        ID = 223;

        Text = "Seeing you have no water of your own, Stooping Eagle says, \"You have not come well prepared into this desert, my friend.\"\n\n\"It seems not.\" You wipe a dusty trickle off your sunburnt brow.\n\nHe hands you the waterskin. \"We are both nobles -- even though from different cities and of different races. It would be churlish of me not to share my rations, since we share a thirst for vengeance.\"\n\nYou are careful to take only a little of the water. It is warm and tastes stale. There is not much left anyway, but you may be glad of the last drop before you reach the palace of Necklace of Skulls.";

        Image = "images/filler1.png";

        Choices.clear();

        Controls = StandardControls();
    }

    int Continue(Character::Base &player) { return 220; }
};

class Story224 : public Story::Base
{
public:
    std::string PreText = "";

    Story224()
    {
        ID = 224;

        Image = "images/filler1.png";

        Choices.clear();

        Controls = StandardControls();
    }

    void Event(Character::Base &player)
    {
        PreText = "The corbels of the roof produce a heart-stopping groan of cracking masonry. Cracks spread across the stone with terrifying speed. You throw yourself forward just as it gives way completely, pelting you with falling rubble.\n\nYou LOSE 3 Life Points.";

        Character::GAIN_LIFE(player, -3);

        if (player.Life > 0)
        {
            PreText += "\n\nYou stagger out into the open in a suffocating cloud of rock dust. You are badly bruised and you are bleeding from several deep cuts. You cough the dust out of your lungs and take a look back along the tunnel. It is now buried under tons of fallen masonry. Another second's hesitation and you'd be buried too.\n\nThe courtiers join you. \"That was nothing,\" sneers their chief. \"We have much more entertaining challenges lined up for you.\"";
        }

        Text = PreText.c_str();
    }

    int Continue(Character::Base &player) { return 431; }
};

class Story225 : public Story::Base
{
public:
    std::string PreText = "";

    Story225()
    {
        ID = 225;

        Text = "You spend the day crouched in the meagre shade afforded by the walls of the courtyard. At sunset, the courtiers lead you to the second of the five windowless buildings. When they open the door, you can see nothing but blackness inside. A waft of acrid air touches your face as you step inside. You get the impression of a high-ceilinged hall whose dark recesses are filled by rustling and high-pitched squeaks. Something like dust brushes your face. You put up your fingers and run them through your hair, then grimace when you see what is falling from the roof: lice.\n\n\"The bats are our master's second favourite pets, after ourselves. They are vampire bats, of course,\" says the chief courtier. He peers in and calls up to the rafters: \"Supper time, gentlemen!\" Then he leaves and the door is slammed shut, blotting out all light.";

        Image = "images/filler1.png";

        Choices.clear();

        Controls = StandardControls();
    }

    int Continue(Character::Base &player)
    {
        if (Character::VERIFY_ITEMS(player, {Item::Type::OWL}))
        {
            return 421;
        }
        else if (Character::VERIFY_CODEWORD(player, Codeword::Type::ZOTZ))
        {
            return 432;
        }
        else
        {
            return 17;
        }
    }
};

class Story226 : public Story::Base
{
public:
    Story226()
    {
        ID = 226;

        Text = "Now it is the opposing team's turn to serve. They send the ball skidding along the side wall in a long arc, and the enemy offensive player comes towards you.";

        Image = "images/filler1.png";

        Choices.clear();
        Choices.push_back(Choice::Base("Move aside and let him pass", 249));
        Choices.push_back(Choice::Base("Make a tackle", 272));
        Choices.push_back(Choice::Base("Retreat ahead of him", 295));

        Controls = StandardControls();
    }
};

class Story227 : public Story::Base
{
public:
    std::string PreText = "";

    Story227()
    {
        ID = 227;

        Image = "images/filler1.png";

        Choices.clear();

        Controls = StandardControls();
    }

    void Event(Character::Base &player)
    {
        PreText = "You raise your wand and stand ready to do battle with Necklace of Skulls. The first exchange of spells is cautious, as each of you probes the other's psychic defences. Then you become bolder, casting crackling fireballs and shuddering bolts of plasma across the intervening space.\n\nThe air itself darkens with the presence of magic. Shapes become warped in the wake of each reality-twisting conjuration. Your foe is visible only as a glimmering speck, like an insect trapped in amber, calling energy up out of the depths of his black soul to hurl at you. A spell cuts through your guard, searing you to the core. You retaliate with a spray of lightning that makes your foe shudder, and he counters by sending a cloud of choking gas towards you.";

        auto DAMAGE = -8;

        PreText += "\n\n";

        if (Character::VERIFY_SKILL(player, Skill::Type::CHARMS))
        {
            DAMAGE = -4;

            PreText += "[CHARMS] ";
        }

        PreText += "You LOSE " + std::to_string(-DAMAGE) + " Life Points.";

        Character::GAIN_LIFE(player, DAMAGE);

        Text = PreText.c_str();
    }

    int Continue(Character::Base &player)
    {
        if (Character::VERIFY_CODEWORD(player, Codeword::Type::VENUS))
        {
            return 442;
        }
        else
        {
            return 342;
        }
    }
};

class Story228 : public Story::Base
{
public:
    Story228()
    {
        ID = 228;

        Text = "Following the sweep of the coastline, you press on into the north-west. You subsist on fruit and fish for a week or so until finally you arrive at the great port of Tahil. The streets are crowded with refugees, all pouring towards the quayside in the hope of finding passage on a ship going east.\n\nYou make your way through the press of frightened people and past the great temples and townhouses which now lie deserted. The causeway to the west is empty apart from a few forlorn stragglers and those who have fallen crippled by the wayside.\n\n\"Turn back!\" cautions a starving beggar as he passes you on the causeway. \"Monsters are coming out of the western desert to slay us all!\"\n\n\"No,\" you reply without looking back, \"I'm going to slay them.\"";

        Image = "images/filler1.png";

        Choices.clear();

        Controls = StandardControls();
    }

    int Continue(Character::Base &player) { return 85; };
};

class Story229 : public Story::Base
{
public:
    std::string PreText = "";

    Story229()
    {
        ID = 229;

        Image = "images/filler1.png";

        Choices.clear();
        Choices.push_back(Choice::Base("Entice it out under the colonnade", 362));
        Choices.push_back(Choice::Base("Give up and choose another route: the pit of coals", 338));
        Choices.push_back(Choice::Base("... the tunnel blocked by wooden beams", 361));
        Choices.push_back(Choice::Base("... the ominously unguarded passage", 382));

        Controls = StandardControls();
    }

    void Event(Character::Base &player)
    {
        PreText = "Spittle flies from the hound's jaw and fury fills its pink eyes as it leaps forward eagerly to fight you. It is keen to chomp your bones and drink the marrow from them.";

        auto DAMAGE = -3;

        PreText += "\n\n";

        if (Character::VERIFY_SKILL_ANY(player, Skill::Type::SWORDPLAY, {Item::Type::SWORD, Item::Type::JADE_SWORD}))
        {
            DAMAGE = -1;

            PreText += "[SWORDPLAY] ";
        }
        else if (Character::VERIFY_SKILL(player, Skill::Type::UNARMED_COMBAT))
        {
            PreText += "[UNARMED COMBAT] ";

            DAMAGE = -2;
        }

        PreText += "You LOSE " + std::to_string(-DAMAGE) + " Life Point(s).";

        Character::GAIN_LIFE(player, DAMAGE);

        if (player.Life > 0)
        {
            PreText += "\n\nAfter a short exchange you are forced back, bleeding, to the mouth of the passage. The hound crouches ready to renew its attack.";
        }

        Text = PreText.c_str();
    }
};

class Story230 : public Story::Base
{
public:
    Story230()
    {
        ID = 230;

        Text = "The spiky-leafed bush is a mescal plant. You can get water from it by biting the tips off the leaves and sucking the moisture out of them. You know that eating any of these plants would only increase your thirst. It is water you need now, not food.\n\nYou LOSE 1 Life Point.";

        Image = "images/filler1.png";

        Choices.clear();

        Controls = StandardControls();
    }

    void Event(Character::Base &player)
    {
        Character::GAIN_LIFE(player, -1);
    }

    int Continue(Character::Base &player) { return 152; };
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
        Choices.push_back(Choice::Base("Make a Donation", 301, Choice::Type::DONATE));
        Choices.push_back(Choice::Base("Leave the Temple", 301));

        Controls = StandardControls();
    }

    void Event(Character::Base &player)
    {
        player.DONATION = 0;
    }
};

class Story232 : public Story::Base
{
public:
    std::string PreText = "";

    Story232()
    {
        ID = 232;

        Image = "images/filler1.png";

        Choices.clear();
        Choices.push_back(Choice::Base("Continue the chase", 278));
        Choices.push_back(Choice::Base("Give up and retrace your steps to find your original route", 160));

        Controls = StandardControls();
    }

    void Event(Character::Base &player)
    {
        PreText = "\"Wait!\" You take a step towards the mysterious girl. She barely glances back, but hurls a peal of laughter over her shoulder and skips away through a sparse bank of ferns. Quickening your pace, you plunge through the undergrowth after her. Her own gait is as light as a dancer's, but even though you break into a run you find yourself unable to catch her.\n\nStumbling into a thickset of thorns, you give a gasp of pain and annoyance as the sharp spines rip your clothing and your flesh.\n\nYou LOSE 1 Life Point.";

        Character::GAIN_LIFE(player, -1);

        if (player.Life > 0)
        {
            PreText += "\n\nWhen you manage to struggle free, the girl is still lingering a little way ahead, hovering luminously in the emerald twilight. Now she turns her shining face and gives you a bolder smile, but along with curiosity you feel a stirring of superstitious dread. This chase is leading you far off your route and into the darker depths of the forest. The image of the shimmering jewel-like figure outlined against the shadows between the trees awakens a disquieting comparison. She reminds you of the bright pattern of a spider hanging in its web.";
        }

        Text = PreText.c_str();
    }
};

class Story233 : public Story::Base
{
public:
    Story233()
    {
        ID = 233;

        Text = "Crouching hidden behind a bank of ferns, you wait patiently until a rabbit comes hopping past. It squats with ears pricked up and nose twitching, barely arm's length from your hiding place. You lob a stone over to the far side of the clearing, and the sudden noise startles the rabbit so that it rushes straight into your clutches. A quick twist ends its struggles, and soon you are roasting your catch over a fire. As you chew at the rangey meat, you reflect on how your artful ways are not only of use in the city.";

        Image = "images/filler1.png";

        Choices.clear();

        Controls = StandardControls();
    }

    int Continue(Character::Base &player) { return 279; }
};

class Story234 : public Story::Base
{
public:
    Story234()
    {
        ID = 234;

        Text = "A young man and woman are brought forward by the priests and led to a shrine at the western edge of the hole. A steep flight of steps descends from the shrine towards a platform covered with sacred glyphs. As golden pectorals are placed over the couple's shoulders, it becomes clear that they are going to be sacrificed. They have chosen to jump into the sinkhole, carrying the people's prayers to the Rain God who dwells under the world.\n\n";

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

class Story235 : public Story::Base
{
public:
    std::string PreText = "";

    Story235()
    {
        ID = 235;

        Image = "images/filler1.png";

        Choices.clear();

        Controls = StandardControls();
    }

    void Event(Character::Base &player)
    {
        PreText = "";

        if (Character::VERIFY_ITEMS(player, {Item::Type::JADE_BEAD}))
        {
            PreText = "You slip a JADE BEAD under your tongue as you were told to do.\n\n";
        }

        PreText += "You steel your nerves and leap from the lip of the sinkhole.\n\nThe water rushes up to meet you, enfolding you in a silent icy embrace. Shock drives the air out of your lungs and you flail wildly. Instantly disoriented, you have no idea which way to swim to reach the surface. Bloody darkness thunders through your brain. You feel yourself drifting, and you know that by now you should have had a glimpse of the sunlit surface of the water. You are not in the bottom of the well anymore. You have plunged into the fabled river that leads between the world of the living and the dead.\n\nYou LOSE 3 Life Points.";

        Character::GAIN_LIFE(player, -3);

        Text = PreText.c_str();
    }

    int Continue(Character::Base &player) { return 119; }
};

class Story236 : public Story::Base
{
public:
    Story236()
    {
        ID = 236;

        Text = "A gust of wind carrying a rotting miasmal stench tells you that you are approaching the end of the tunnel. The demons steer along a side passage towards a patch of grey daylight, emerging under a sky the colour of wet limestone. This tributary of the river is barely more than a muddy trickle. The rank smell hangs over a dreary expanse of marshland which stretches off into the distance. No matter which way you look, all you can see is a landscape of sour white clay covered with scum-covered ponds and grey tufts of reeds.\n\nYou put in at a rotting wooden jetty and the demons wait for you to disembark. \"Hope you enjoyed the voyage,\" cackles one.\n\n\"It's customary to show your appreciation,\" says the other as you clamber onto the jetty.\n\n\"That's right!\" says the first as though it has only just occurred to him. \"Got a JADE BEAD you could let us have?\"";

        Image = "images/filler1.png";

        Controls = StandardControls();
    }

    void Event(Character::Base &player)
    {
        Choices.clear();

        if (!Character::VERIFY_SKILL(player, Skill::Type::FOLKLORE))
        {
            Choices.push_back(Choice::Base("Give them JADE BEAD", 53, Choice::Type::GIVE_ITEM, Item::Type::JADE_BEAD));
            Choices.push_back(Choice::Base("Continue", 53));
        }
    }

    int Continue(Character::Base &player) { return 31; }
};

class Story237 : public Story::Base
{
public:
    std::string PreText = "";

    Story237()
    {
        ID = 237;

        Image = "images/filler1.png";

        Choices.clear();

        Controls = StandardControls();
    }
    void Event(Character::Base &player)
    {
        PreText = "";

        if (Character::VERIFY_ITEMS(player, {Item::Type::JADE_BEAD}))
        {
            PreText += "The jade bead rolls out from under your tongue when you speak and it falls into the water.\n\n";
        }

        PreText += "\"There's no other way,\" replies the demon with the jaguar-skin skullcap.\"\n\n\"Not if you want to reach the Deathlands alive,\" adds his accomplice, and they rock with immoderate joy like two senile old men.";

        Text = PreText.c_str();
    }

    int Continue(Character::Base &player) { return 261; }
};

class Story238 : public Story::Base
{
public:
    Story238()
    {
        ID = 238;

        Text = "You join the queue of people waiting outside the palace in the hope of being granted an audience. When it comes to your turn, a snooty courtier soon makes it plain that you will have to bribe him if you want your gift taken to the King.";

        Image = "images/filler1.png";

        Choices.clear();
        Choices.push_back(Choice::Base("Bribe him (3 cacao)", 285, Choice::Type::LOSE_MONEY, 3));
        Choices.push_back(Choice::Base("An audience with the King is not worth it", 262));

        Controls = StandardControls();
    }
};

class Story239 : public Story::Base
{
public:
    Story239()
    {
        ID = 239;

        Text = "Something passes overhead with a heavy flapping noise, then descends to alight on the spire where you are clinging. You look up to see the familiar face of the dwarf you helped earlier. Ugly as he is, you could not imagine a more joyous sight -- for you now see that his long ares are not clothed in a black mantle as you had supposed, but are actually bat-like wings.\n\n\"Grab hold of my feet,\" he says.\n\nYou do not need to be told twice. Once you have a firm grip, he sweeps out his wings and flies you across the canyon. It is a breathtaking ride, swooping with giddying speed high above the billowing lava-mist as the far edge of the canyon comes rushing nearer. You are not sorry when Zotz finally spirals down and your feet touch solid ground again.\n\n\"There, I said I'd repay my debt,\" says Zotz with a grotesque grin. \"Take care, now!\" And he leaps off the edge, gliding back until he is swallowed by the rising veils of steam.";

        Image = "images/filler1.png";

        Choices.clear();

        Controls = StandardControls();
    }

    void Event(Character::Base &player)
    {
        Character::REMOVE_CODEWORD(player, Codeword::Type::ZOTZ);
    }

    int Continue(Character::Base &player) { return 263; }
};

class Story240 : public Story::Base
{
public:
    Story240()
    {
        ID = 240;

        Text = "Necklace of Skulls towers over you, his huge face looming like a grotesque white mushroom against the indigo blue of the sky. His sword descends in a murderous arc. You throw yourself to one side and it clashes against a block masonry, scattering chips of broken stone. And physical strength is not his only weapon -- he also unleashes sorcery, spewing out torrents of acrid smoke from his mouth. His screams of fury are so charged with magic that they awaken the skulls hanging at his chest. Chittering with malevolence, they strain on their cords to snap at you.\n\nIt is an impressive display of sheer power. Others might even find it terrifying, but you have come too far and faced too many perils to fail now. You stand side by side with your brother and slowly force the wizard back. Just as his fury takes tangible form, so his desperation shows as sparks of cindery light. He stabs out with his sword a final time, wounding Morning Star, then emits a bleak cry like a dying vulture and topples back into the pit leading to the interior of the pyramid.\n\nAs your foe dies, the whole palace begins to shudder. You help Morning Star to his feet, relieved to see that his wound is not a fatal one. He will always carry a scar to remind him of Necklace of Skulls' final sword-strike, but at least he will live.\n\n\"I think we ought to get out of here,\" you say to him as a nearby building caves in.\n\n\"I think you're right,\" he says.";

        Image = "images/necklace-of-skulls.png";

        Choices.clear();

        Controls = StandardControls();
    }

    int Continue(Character::Base &player) { return 442; }
};

class Story241 : public Story::Base
{
public:
    Story241()
    {
        ID = 241;

        Text = "The MAN OF GOLD draws life from your body warmth. Dropping to the causeway, he goes running towards the monster. It stares down wide-eyed at his approach, unable to believe that a tiny metal manikin could present much of a threat. Snorting with unhuman laughter, it strides forward and tries to kick the MAN OF GOLD aside.\n\nThat is a mistake. Bodily uprooting a chunk of stone from the causeway, the MAN OF GOLD brings it crashing down a surprised yelp as he lifts its huge bulk clear of the ground and sends it hurtling off to land in the maggot-infested mire surrounding the causeway.\n\nWith a gesture of farewell, the MAN OF GOLD runs on to the jetty. By the time you catch up, he has plunged without a trace into the gelid green waters.";

        Image = "images/filler1.png";

        Choices.clear();

        Controls = StandardControls();
    }

    void Event(Character::Base &player)
    {
        Character::LOSE_ITEMS(player, {Item::Type::MAN_OF_GOLD});
    }

    int Continue(Character::Base &player) { return 20; }
};

class Story242 : public Story::Base
{
public:
    std::string PreText = "";

    Story242()
    {
        ID = 242;

        Image = "images/filler1.png";

        Choices.clear();

        Controls = StandardControls();
    }

    void Event(Character::Base &player)
    {
        PreText = "You duck to one side as the monster comes rushing forward. It stumbles past, but throws out its arm and catches you a powerful blow. You wince as you hear one of your ribs crack, and the pain sends you staggering back out from under the trees into the hot afternoon sunshine.";

        PreText += "\n\n";

        if (Character::VERIFY_SKILL(player, Skill::Type::AGILITY))
        {
            PreText += "[AGILITY] You turn away from the brunt of the blow.";
        }
        else
        {
            PreText += "You LOSE 2 Life Points.";

            Character::GAIN_LIFE(player, -2);
        }

        Text = PreText.c_str();
    }

    int Continue(Character::Base &player) { return 265; }
};

class Story243 : public Story::Base
{
public:
    Story243()
    {
        ID = 243;

        Text = "The road leads you through a hazy realm. You pass by ranks of tall stately figures with bald elongated heads and cross-eyed expressions that make them seem introspective and wistful. You try to speak to them, to ask where you are, but they recede into the distance whenever you approach.\n\nYou cannot tell how long has passed when you find yourself back at the crossroads. Your memory is cloudy, and you realize that you have lost some of your expertise, along with other recollections.\n\nOne of the other paths must be the correct one.";

        Image = "images/filler1.png";

        Choices.clear();
        Choices.push_back(Choice::Base("Follow the red road", 196));
        Choices.push_back(Choice::Base("Follow the black road", 219));
        Choices.push_back(Choice::Base("Follow the yellow road", 266));

        Controls = StandardControls();
    }

    void Event(Character::Base &player)
    {
        if (player.Skills.size() > 1)
        {
            LimitSkills = player.Skills.size() - 1;
        }
        else
        {
            LimitSkills = 0;
        }

        Character::REMOVE_CODEWORD(player, Codeword::Type::POKTAPOK);
    }
};

class Story244 : public Story::Base
{
public:
    Story244()
    {
        ID = 244;

        Text = "\"I'll help you find out their names,\" says a familiar high-pitched voice in your ear.\n\nStartled you look around but there is no one there. Then a flicker of motion in the corner of your eye draws your attention to a tiny hovering shape. It flits down to alight on your hand.\n\nSquinting at it, you are astonished to find it is the tiny wizened fellow with the long nose whom you helped when you first arrived in the underworld. Viewed at this size, he looks as much like a mosquito as a man.\n\nZaz goes flying off down the passage. \"Ouch!\" says the first sentinel after a moment.\n\n\"What is it, Grandfather of Darkness?\" enquires the next sentinel.\n\n\"Something bit me, Thunderbolt Laughter.\" He leans out of his alcove and calls to the third sentinel, \"Can you see a mosquito flying around, Lord Blood?\"\n\nLord Blood's reply is interrupted by a yelp of pain from the last sentinel. He turns and asks: \"Did it bite you, Lord Skull?\"\n\nWhile the sentinels start grumbling about the mosquito, you make your way along the tunnel. You address each of them by the names that Zaz cunningly tricked them into revealing, and so you are allowed to pass unmolested to the exit.";

        Image = "images/filler1.png";

        Choices.clear();

        Controls = StandardControls();
    }

    void Event(Character::Base &player)
    {
        Character::REMOVE_CODEWORD(player, Codeword::Type::ZAZ);
    }

    int Continue(Character::Base &player) { return 336; }
};

class Story245 : public Story::Base
{
public:
    Story245()
    {
        ID = 245;

        Text = "Three of its heads stab forward and the fourth sways aloft, surveying your every move. You barely evade the gnashing fangs as hot venom splashes into the sand where you were standing.";

        Image = "images/filler1.png";

        Choices.clear();
        Choices.push_back(Choice::Base("Continue to dodge back", 314));
        Choices.push_back(Choice::Base("You think that now is the right moment to charge in and attack", 337));

        Controls = StandardControls();
    }
};

class Story246 : public Story::Base
{
public:
    std::string PreText = "";

    Story246()
    {
        ID = 246;

        Image = "images/filler1.png";

        Choices.clear();

        Controls = StandardControls();
    }

    void Event(Character::Base &player)
    {
        PreText = "You cannot keep a look of covetous disappointment off your face when Stooping Eagle replaces the stopper without offering you a drink. \"You came ill-prepared for a desert crossing, it seems,\" he says as he replaces the waterskin at his belt.\n\nYou can only nod. Your tongue is too dry to waste words. You wipe a dusty trickle of sweat off your sunburnt brow and smear the salty moisture across your lips.\n\nStooping Eagle adopts a look of regret. \"If only you were a nobleman like myself, I would be happy to share my rations with you, meagre as they are. But there are certain standards we must maintain even in the face of death. A noble does not drink from the same flask as a commoner.\"\n\n\"Our skeletons won't look much different when they've both been bleached by the sun's rays,\" you reply sullenly.\n\nYou would argue the point further, but if the two of you came to blows here and now it would just waste your last reserves of strength. The only victor in such a struggle would be the merciless sun and the uncaring sands.";

        auto DAMAGE = -2;

        PreText += "\n\n";

        if (Character::VERIFY_SKILL(player, Skill::Type::WILDERNESS_LORE))
        {
            DAMAGE = -1;

            PreText += "[WILDERNESS LORE] ";
        }

        PreText += "You LOSE " + std::to_string(-DAMAGE) + " Life Points.";

        Character::GAIN_LIFE(player, DAMAGE);

        if (player.Life > 0)
        {
            if (Character::VERIFY_SKILL(player, Skill::Type::WILDERNESS_LORE))
            {
                PreText += "\n\n[WILDERNESS LORE] You have a natural hardiness that prevented further LOSS of Life Points.";
            }
        }

        Text = PreText.c_str();
    }

    int Continue(Character::Base &player) { return 220; }
};

class Story247 : public Story::Base
{
public:
    Story247()
    {
        ID = 247;

        Text = "You once heard a folktale about an albino hound that guarded a dead king's treasure. The hero of the story defeated the hound by luring it into the open, where its weak eyes were blinded by sunlight. You glance back over your shoulder. Outside the colonnade, the sun is so bright that it makes your eyes hurt. You would have a definite advantage over the hound if you got it to follow you out there. On the other hand, looking at the hound, maybe 'less of a disadvantage' would be a more accurate phrase. You reckon it to be about a hundred kilograms of bad-tempered bone and muscle.";

        Image = "images/filler1.png";

        Choices.clear();
        Choices.push_back(Choice::Base("Fight it", 362));
        Choices.push_back(Choice::Base(Skill::CUNNING.Name, 270, Skill::Type::CUNNING));
        Choices.push_back(Choice::Base(Skill::TARGETING.Name, 316, Skill::Type::TARGETING));
        Choices.push_back(Choice::Base(Item::Descriptions[Item::Type::HYDRA_BLOOD_BALL], 383, Item::Type::HYDRA_BLOOD_BALL));

        Controls = StandardControls();
    }
};

class Story248 : public Story::Base
{
public:
    Story248()
    {
        ID = 248;

        Text = "You feel as though you are about to be sick. You are surprised and ashamed. You have felt fear before, but you have always borne it bravely. Chattering teeth, quaking limbs and nausea might be excusable in a small child, but not in a proud Maya warrior.\n\nWith an uncontrollable belch, your stomach suddenly ejects something up into your throat. You feel it squirming. It's alive! Your mouth drops open in amazement and a firefly buzzes out.\n\nThe firefly circles the candle just as a gust of air extinguishes the flame. For a moment you are plunged into darkness, and your skin crawls with primordial fear as you imagine the spirits of the dead rising from their grave-mounds. But then the firefly settles on the wick and gives off a glow that makes it seem that the candle is still burning.\n\nThe courtiers are openly dumbfounded when you emerge from the House of Gloom at dawn showing every sign of having spent a peaceful night. \"Were you not afflicted by ghastly nightmares, visitations, hauntings and mind-shattering terrors?\" asks the chief courtier.\n\n\"Not at all. Here's your candle back,\" you reply.\n\nHe stares at it. \"It hasn't even burned down!\"\n\n\"You didn't leave me anything to read,\" you say with a shrug, \"so I just blew it out and got some sleep.\"\n\nYou pretend not to notice his expression of astonishment as you retrieve your pack of belongings.\n\nYou RECOVER 1 Life Point.";

        Image = "images/filler1.png";

        Choices.clear();

        Controls = StandardControls();
    }

    void Event(Character::Base &player)
    {
        Character::GAIN_LIFE(player, 1);
    }

    int Continue(Character::Base &player) { return 40; }
};

class Story249 : public Story::Base
{
public:
    Story249()
    {
        ID = 249;

        Text = "He advances past you and presses on into your own team's defensive zone.";

        Image = "images/filler1.png";

        Choices.clear();
        Choices.push_back(Choice::Base("Chase after him", 341));
        Choices.push_back(Choice::Base("Run towards the enemy defence", 318));
        Choices.push_back(Choice::Base("Stay in mid-arena", 364));

        Controls = StandardControls();
    }
};

class Story250 : public Story::Base
{
public:
    Story250()
    {
        ID = 250;

        Text = "You do not even get a dozen paces. Contemptuously, Necklace of Skulls raises one of his many-jointed limbs and brings a gout of celestial darkness streaming down from the cobalt sky. You are engulfed in icy shadow, and can only writhe in silent horror as the spell sucks you out of this world and carries you down through the ground towards the abode ghosts. You have failed.";

        Image = "images/necklace-of-skulls.png";

        Type = Story::Type::DOOM;

        Choices.clear();

        Controls = StandardControls();
    }
};

class Story251 : public Story::Base
{
public:
    Story251()
    {
        ID = 251;

        Text = "You find a trader who will shortly be sailing up the coast to Tahil.\n\nHe offers a free passage if you will help defend the boat against pirates. If not, he will charge you 6 cacao passage.";

        Image = "images/filler1.png";

        Choices.clear();
        Choices.push_back(Choice::Base("Travel with him and help with the defence (SWORD and BLOWGUN)", 300, Choice::Type::ALL_ITEMS, {Item::Type::SWORD, Item::Type::BLOWGUN}));
        Choices.push_back(Choice::Base("... or with a JADE SWORD and BLOWGUN", 300, Choice::Type::ALL_ITEMS, {Item::Type::JADE_SWORD, Item::Type::BLOWGUN}));
        Choices.push_back(Choice::Base("Pay for your passage (6 cacao)", 300, Choice::Type::LOSE_MONEY, 6));
        Choices.push_back(Choice::Base("Travel overland instead", 228));

        Controls = StandardControls();
    }
};

class Story252 : public Story::Base
{
public:
    Story252()
    {
        ID = 252;

        Text = "It is an arduous climb in the dry noon heat. You reach the city exhausted, and are grateful for the goblet of water that is put into your hands by a smiling priest. He beckons to a throng of richly attired warriors whose red face-paint and black helmet feathers make their welcoming smiles look rather fierce. \"We're glad to greet such an esteemed guest,\" says one.\n\n\"You are just the sort of person we're looking for these days,\" mutters another, hand resting casually on his sword-hilt.\n\n\"Why not come this way and spruce yourself up?\" says another, resting his strong arm across your weary shoulders.\n\nYou are led to the altar platform on top of the city's main pyramid, where more smiling priests await you. And if you feel like a turkey who's being invited to a feast, you are not far wrong.";

        Image = "images/filler1.png";

        Type = Story::Type::DOOM;

        Choices.clear();

        Controls = StandardControls();
    }
};

class Story253 : public Story::Base
{
public:
    Story253()
    {
        ID = 253;

        Text = "You slice off the top of one of the cacti and drink its sap. The taste is unpleasantly bitter. You manage to resist the urge to vomit, knowing that to do so now would mean your death. Resting until your stomach stops gurgling, you head on across the barren sun-bleached land.\n\nYou LOSE 3 Life Points.";

        Image = "images/filler1.png";

        Choices.clear();

        Controls = StandardControls();
    }

    void Event(Character::Base &player)
    {
        Character::GAIN_LIFE(player, -3);
    }

    int Continue(Character::Base &player) { return 152; }
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
        Choices.push_back(Choice::Base("You are not prepared to make any donation and you need to hurry over to the market and spend your money on supplies instead", 93));

        Controls = StandardControls();
    }
};

class Story255 : public Story::Base
{
public:
    Story255()
    {
        ID = 255;

        Text = "The stabai are inconstant creatures. Their perspective is like that of the forest itself, where the promises and threats of mankind mean nothing beside the endless cycle of death, decay and rebirth. The only way you will get anything form them is by keeping a tight grip on the SHAWL. As long as you hold the stabai's precious property, you have some power over them. Relinquish it, and you immediately lay yourself open to their most noxious tricks.";

        Image = "images/filler1.png";

        Choices.clear();

        Controls = StandardControls();
    }

    int Continue(Character::Base &player) { return 369; }
};

class Story256 : public Story::Base
{
public:
    Story256()
    {
        ID = 256;

        Text = "The rumbling in your belly grows more insistent. You must find something to eat or you risk starving here in the forest's depths.";

        Choices.clear();

        Controls = StandardControls();
    }

    void Event(Character::Base &player)
    {
        Choices.clear();

        if (Character::COUNT_ITEMS(player, {Item::Type::MAIZE_CAKES, Item::Type::PAPAYA, Item::Type::OWL, Item::Type::HAUNCH_OF_VENISON, Item::Type::SALTED_MEAT}))
        {
            Choices.push_back(Choice::Base("Eat from your provisions", 279, Choice::Type::EAT_HEAL, {Item::Type::MAIZE_CAKES, Item::Type::PAPAYA, Item::Type::OWL, Item::Type::HAUNCH_OF_VENISON, Item::Type::SALTED_MEAT}, 1));
        }
        else
        {
            Choices.push_back(Choice::Base("You have nothing to eat", 4));
        }
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
        Choices.push_back(Choice::Base("Resist the fate they have in store for you", 281));
        Choices.push_back(Choice::Base("[SPELLS] Cast a protective enchantment", 304, Choice::Type::SKILL_ANY, Skill::Type::SPELLS, {Item::Type::MAGIC_WAND, Item::Type::JADE_SWORD}));
        Choices.push_back(Choice::Base("Agree to being thrown into the pit", 327));

        Controls = StandardControls();
    }
};

class Story258 : public Story::Base
{
public:
    Story258()
    {
        ID = 258;

        Text = "The strange pair guide their canoe into a tunnel leading off the side of the cavern. As you proceed, the roof of the tunnel gets lower until finally you have to crouch down to avoid bumping your head. The tunnel is so narrow by now that the sides of the canoe are scraping against the rock walls. You begin to worry that you will get wedged in the tunnel, unable either to go on or turn back, but the two unhuman oarsmen are entirely unconcerned. Pressed into the bottom of the boat, you can hear them sniggering to themselves.\n\n\"The water level's higher than the last time,\" calls back the one in front.\n\n\"It's a tight squeeze,\" agrees the other. \"We might have to go under.\"\n\nGo under? Do they mean submerge?";

        Image = "images/filler1.png";

        Choices.clear();
        Choices.push_back(Choice::Base("Tell them to take you back to the cave and find another route", 237));
        Choices.push_back(Choice::Base("Let them row on", 361));

        Controls = StandardControls();
    }
};

class Story259 : public Story::Base
{
public:
    Story259()
    {
        ID = 259;

        Text = "You reach the cliff. It is a solid wall of smooth grey stone receding off towards the horizon to left and right, and bounded above by the shimmering surface of the water hanging overhead. There are two doorways into the cliff, each sealed by a gate of stout wooden bars held shut by an elaborate knot of rope as thick as your wrist.";

        Image = "images/filler1.png";

        Choices.clear();

        Controls = StandardControls();
    }

    int Continue(Character::Base &player)
    {
        if (player.Money > 0)
        {
            return 414;
        }
        else
        {
            return 358;
        }
    }
};

class Story260 : public Story::Base
{
public:
    Story260()
    {
        ID = 260;

        Text = "You walk along the shore as dusk gathers and the stars slowly emerge against the curtain of night. Ahead of you, nestling at the base of the cliffs, you see a massive round head that seems to be carved out of smooth black stone. It is taller than a man. As you get closer, it becomes possible to make out the features: a strong face with wide aristocratic nose, thick lips compressed in stern deliberation, heavy brows above eyes which stare impassively out to sea.\n\nThen you realize you can hear muttering. A low quiet sound at the very limit of audibility. It sounds like someone counting: \"Seventeen million and sixty-two, seventeen million and sixty-three...\"\n\nYou step up to the head and say, \"Excuse me.\"\n\nThe huge eyes roll in their sockets with a stony scraping. You find yourself fixed with a disconcerting stare. The eyes hold that blank expression which lies on the far side of outrage and disbelief.\n\nAfter a moment, the head's gaze turns back to the starry sky. \"One,\" you hear it say distinctly. \"Two. Three...\"\n\nYou give a polite cough. \"There are one hundred thousand million and seven of them,\" you venture.\n\nThe huge eyes swivel back to study you again, this time filled with a look of cautious hope. \"You're sure? I thought mortal eyes could only see a few thousand stars.\"\n\n\"They can, but I was told the number by a magician.\"\n\nHe gives a gravelly sigh. \"I have been counting the stars since before the coming of man -- but they kept moving, and often the daytime made me lose count. See, I've been here so long I've been buried up to my neck.\"\n\nYou look at the sand and gravel, trying to imagine the huge body buried beneath it.";

        Image = "images/giant.png";

        Choices.clear();
        Choices.push_back(Choice::Base("Ask the giant for a favour", 23));
        Choices.push_back(Choice::Base("It is time to get on with planning your journey to Tahil", 113));

        Controls = StandardControls();
    }

    void Event(Character::Base &player)
    {
        if (Character::VERIFY_CODEWORD(player, Codeword::Type::OLMEK))
        {
            Character::REMOVE_CODEWORD(player, Codeword::Type::OLMEK);
        }
    }
};

class Story261 : public Story::Base
{
public:
    Story261()
    {
        ID = 261;

        Text = "Just when it seems certain that the canoe will get stuck in the passage, immuring you below the earth for ever, you realize that the gap is widening. The canoe drifts on into a vast tunnel through which runs a wide underground river. Misty blue light sparkles on the water and trickles across the glistening rock. It appears to emanate from veins of glassy stone which you can see running through the walls of the tunnel.\n\nThe demons manoeuvre their craft between a forest of stalagmites which protrude from the water like thin fangs. Once on the open river they begin to ply their oars with vigour, propelling the canoe amid whoops of crazed glee.\n\nYou gaze in awe at the wondrous sight surrounding you. The tunnel is far wider than any stream to be found in the dry countryside around Koba, with walls rising almost vertically to a shadow-filled roof a hundred metres above your head. The air here is hot and musty and has a vile taste that makes you cough, but other than that you could almost imagine you are being steered along a canyon in the open air.\n\nRounding a bend in the river, you notice a series of stone doors set off a ledge high up in the right-hand wall of the tunnel. \"The cave tombs of the first ancestors,\" says the demon in the back of the boat when he sees where you are looking.\n\n\"I expect you'll want to take a closer look,\" says the other demon and, without waiting for a reply, they row over to the side of the river and steady the canoe below the ledge.";

        Image = "images/filler1.png";

        Choices.clear();
        Choices.push_back(Choice::Base("[AGILITY] Climb up to the tombs", 190, Skill::Type::AGILITY));
        Choices.push_back(Choice::Base("Use magic to get up there (SPELLS and ROPE)", 213, Skill::Type::SPELLS, Item::Type::ROPE));
        Choices.push_back(Choice::Base("Do not to explore the tombs", 236));

        Controls = StandardControls();
    }
};

class Story262 : public Story::Base
{
public:
    Story262()
    {
        ID = 262;

        Text = "You decide on another course of action.";

        Image = "images/filler1.png";

        Choices.clear();
        Choices.push_back(Choice::Base("[CUNNING] Try one of your devious schemes", 308, Skill::Type::CUNNING));
        Choices.push_back(Choice::Base("Pay for lodging if you have some money", 101, Choice::Type::MONEY, 1));
        Choices.push_back(Choice::Base("Continue on westwards", 8));
        Choices.push_back(Choice::Base("Follow the river northwards", 30));

        Controls = StandardControls();
    }
};

class Story263 : public Story::Base
{
public:
    Story263()
    {
        ID = 263;

        Text = "You trudge on through terrain consisting of bare bleached rocks swathed in stream rising from fissures in the ground. Wet gravel crunches underfoot. Sweat soaks your clothes, and the air is so hot that you can hardly breathe.\n\nYou see someone sprawled atop a boulder. He is a gangling figure with a weather beaten face and lazy heavy-lidded eyes. Your first impression is that he is asleep, but then he calls out in a sibilant voice, saying, \"You are Evening Star, are you not? I might know a secret or two that could help you find your brother, if you can give me an answer to this riddle. \"I'm a narrow fellow and I live in narrow spaces between the rocks. Born from a pebble, I'm as hard to catch as a flicker of lightning when my blood's up, but in the cool of night I'm as sedentary as a stalactite.\"\n\nWhat answer will you give?";

        Image = "images/filler1.png";

        Choices.clear();
        Choices.push_back(Choice::Base("'A lizard'", 376));
        Choices.push_back(Choice::Base("'A dragonfly'", 397));
        Choices.push_back(Choice::Base("'Water'", 10));
        Choices.push_back(Choice::Base("None of these", 60));

        Controls = StandardControls();
    }
};

class Story264 : public Story::Base
{
public:
    Story264()
    {
        ID = 264;

        Text = "You fall in with others who are travelling in a group for safety. Since the collapse of the great city there has been a wave of refugees from the north-west, many of them impoverished and desperate. It is no longer wise to travel the back roads unaccompanied.\n\nSome of your companions make their farewells as they arrive at their homes, others join the group. You might be walking with up to a dozen other people at any one time, while on other stretches of the riverside path you travel alone. At such times you are keen for company, and when you see a peasant woman walking ahead you quicken your pace to catch up.\n\nYou soon begin to regret joining her, because there is something strange about her manner that gives you a feeling of disquiet despite the bright sunny morning. She peers constantly ahead of her with a dreamy expression, stumbling along as though half asleep. For the sake of conversation, you remark on the large clay pitcher she carries balanced upside-down on her shoulder. \"Isn't it easier to carry those on your head? That's what most peasants do.\"\n\nYour question takes a while to sink in. When her answer comes it is a distracted murmur: \"Only if it's full... This isn't full...\"\n\nYou walk on for several minutes before saying, \"Why don't you switch it to your other shoulder? You'd find it less of a strain that way, I'm sure.\"\n\n\"It's fine like this...\" She suddenly stops and turns to you with a drowsy smile. \"I think I'll rest in the shade of this tree. You'll wake until I wake up, won't you? It's too hot to walk in the middle of the day anyhow...\"\n\nBefore you can reply, she hunkers down by the side of the road -- still with the pitcher balanced carefully on her shoulder -- and her head slumps forward in sleep.";

        Image = "images/filler1.png";

        Controls = StandardControls();
    }

    void Event(Character::Base &player)
    {
        Choices.clear();

        if (!Character::VERIFY_SKILL(player, Skill::Type::FOLKLORE) && !Character::VERIFY_CODEWORD(player, Codeword::Type::CALABASH))
        {
            Choices.push_back(Choice::Base("Stay here as the woman asked you to", 100));
            Choices.push_back(Choice::Base("Sneak a look under a pitcher", 333));
            Choices.push_back(Choice::Base("Leave before she wakes up", 356));
        }
    }

    int Continue(Character::Base &player)
    {
        if (Character::VERIFY_SKILL(player, Skill::Type::FOLKLORE))
        {
            return 287;
        }
        else if (Character::VERIFY_CODEWORD(player, Codeword::Type::CALABASH))
        {
            return 310;
        }

        return -1;
    }
};

class Story265 : public Story::Base
{
public:
    std::string PreText = "";

    Story265()
    {
        ID = 265;

        Image = "images/filler1.png";

        Choices.clear();

        Controls = StandardControls();
    }

    void Event(Character::Base &player)
    {
        PreText = "The monster rushes forward, realizing too late that it has been tricked. Once out of the shade of the tree where it was resting, it is dazzled by the bright sunlight and can only flail back blindly as you step in to finish it. Even so, its clumsy blows strike you with staggering force.";

        auto DAMAGE = -4;

        PreText += "\n\n";

        if (Character::VERIFY_SKILL_ANY(player, Skill::Type::SWORDPLAY, {Item::Type::SWORD, Item::Type::JADE_SWORD}))
        {
            DAMAGE = -1;

            PreText += "[SWORDPLAY] ";
        }
        else if (Character::VERIFY_SKILL(player, Skill::Type::UNARMED_COMBAT))
        {
            PreText += "[UNARMED COMBAT] ";

            DAMAGE = -2;
        }

        PreText += "You LOSE " + std::to_string(-DAMAGE) + " Life Points.";

        Character::GAIN_LIFE(player, DAMAGE);

        Text = PreText.c_str();
    }

    int Continue(Character::Base &player) { return 288; }
};

class Story266 : public Story::Base
{
public:
    Story266()
    {
        ID = 266;

        Text = "As you walk, you pass through bands of bright golden light interspersed by shadow. The flickering effect leaves you dazed and disoriented, so you are slow to react when something heavy slams into your back, forcing you down. You hear a deep resonant growl of a jaguar. Despite your fear, you struggle to rise. Hands -- or paws? -- fumble at your pack. You get to your feet in time to catch a fleeting glimpse of a large feline shape bounding off into the gloom.\n\nYou examine your possessions and find you have lost everything except for one item which you managed to hold on to. You have also been robbed of all your money.\n\nAngrily you retrace your steps to the crossroads and select a different route.";

        Image = "images/filler1.png";

        Choices.clear();
        Choices.push_back(Choice::Base("Follow the red path", 196));
        Choices.push_back(Choice::Base("Follow the white path", 243));
        Choices.push_back(Choice::Base("Follow the black path", 219));

        Controls = StandardControls();
    }

    void Event(Character::Base &player)
    {
        ToLose.clear();

        if (player.Items.size() > 1)
        {
            for (auto i = 0; i < player.Items.size(); i++)
            {
                ToLose.push_back(player.Items[i]);
            }

            Limit = 1;
        }
        else
        {
            Limit = 0;
        }

        player.LostMoney += player.Money;

        player.Money = 0;
    }
};

class Story267 : public Story::Base
{
public:
    Story267()
    {
        ID = 267;

        Text = "According to legend, the hero-twins called Forethought and Afterthought once travelled west across the great desert in search of the tunnel leading to the underworld. There they had to pass these four sentinels. They addressed each with due deference, calling the first Lord Skull, the second Lord Blood, the third Thunderbolt Laughter, and the fourth Grandfather of Darkness. Thus they finally penetrated of the underworld.\n\nMake your you know the detail of the legend.";

        Image = "images/filler1.png";

        Choices.clear();

        Controls = StandardControls();
    }

    int Continue(Character::Base &player) { return 313; }
};

class Story268 : public Story::Base
{
public:
    std::string PreText = "";

    Story268()
    {
        ID = 268;

        Image = "images/filler1.png";

        Choices.clear();

        Controls = StandardControls();
    }

    void Event(Character::Base &player)
    {
        PreText = "The monster dips one of its necks close to the ground and swings it around behind your legs while another lunges towards your face.\n\n";

        if (Character::VERIFY_SKILL(player, Skill::Type::AGILITY))
        {
            PreText += "You jump back over the neck that is trying to trip you while simultaneously ducking the attack of the other.";
        }
        else
        {
            Character::GAIN_LIFE(player, -2);

            PreText += "You caught a staggering blow and it is only by falling backwards that you avoid having your head torn off.\n\nYou LOSE 1 Life Point.";
        }

        Text = PreText.c_str();
    }

    int Continue(Character::Base &player)
    {
        if (Character::VERIFY_SKILL(player, Skill::Type::AGILITY))
        {
            return 337;
        }
        else
        {
            return 222;
        }
    }
};

class Story269 : public Story::Base
{
public:
    Story269()
    {
        ID = 269;

        Text = "Stooping Eagle and his servant are led off across the courtyard towards a group of buildings. \"They will be our guests also, but in another part of the palace from you,\" says the chief of the courtiers, smiling to display a sharp set of teeth.\n\n\"Do not worry, friend,\" Stooping Eagle calls back to you, \"we have only to persevere and our swords shall drink the fiend's blood eventually!\"\n\nYou would like to resists the courtiers, but there too many to fight in your weakened state. \"After five nights I will be taken to Necklace of Skulls?\" you ask. It occurs to you that five nights' rest will leave you all the fitter to deal with the wizard.\n\nThe chief courtier dips his head. \"Exactly. Our master lives in the inner precinct of the palace.\" He gestures with a thin hairy hand towards a pyramid that towers over the inner courtyard. The black colouring of the pyramid makes it look like a crack of the night sky that has lingered on after sunrise.\n\n\"Take me to my quarters, then,\" you tell him.\n\nThe assembled courtiers give a high howling laugh at this. \"Not so fast,\" titters their chief when he has recovered himself. \"First you have to choose your route to our compound.\"";

        Image = "images/filler1.png";

        Choices.clear();

        Controls = StandardControls();
    }

    int Continue(Character::Base &player) { return 315; }
};

class Story270 : public Story::Base
{
public:
    Story270()
    {
        ID = 270;

        Text = "The albino hound leaps forward with a savage bark as it sees you approach. Suddenly jumping back, you lure it out of the passage and into the bright sunshine, where its weak eyes are blinded by the glare. Making sure to keep up a stream of taunts so that it can hear you, you double around and race back towards the unguarded tunnel.\n\nThe hound lopes after you, furiously intent on rending your flesh in its powerful jaws. Dazzled, it does not see you stop on one side of the tunnel entrance. You toss a pebble into the open tunnel and the hound, hearing this, bounds off along it thinking you are still in full flight.\n\nThe arch of the tunnel shudders and gives way, burying the hound under a mass of falling masonry. Once the dust clears there is no sign of it. You go along to the passage previously guarded by the hound and make your way through to the inner courtyard, where you find the courtiers already waiting for you.\n\n\"Clever,\" remarks the chief courtier. \"You'll need more tricks like that if you're going to get through the real tests, though.\"";

        Image = "images/filler1.png";

        Choices.clear();

        Controls = StandardControls();
    }

    int Continue(Character::Base &player) { return 431; }
};

class Story271 : public Story::Base
{
public:
    Story271()
    {
        ID = 271;

        Text = "You scrape some clay out of the walls and spit into it, smearing it into a grey-white mixture which you apply to your eyelids. Once that has dried, you collect a little soot from the candle-flame and dab a black spot into the middle of each eyelid. You have no mirror in which to check the finished result, but it ought to now look as though your eyes are open even when you in fact have them screwed tight shut.\n\nSettling down with your back to the wall, you pull your cloak across in front of you like blanket. After a few minutes, a gust of cool dry air blows out of the candle and you are left in darkness,. As if on cue, the ghosts come seeping up from their graves under the floor. You see flickers of luminosity sketching the outlines of skeletal bodies and grotesque dead faces against the darkness. As they draw near, you close your eyes.\n\n\"Ah, we have a visitor among us,\" whispers a voice like wind sighing in a well. \"Watch, brothers, as I unleash my most terrifying visage.\"\n\nThere is a revolting wet sound a a blaze of grey-blue light that you see even through your closed eyelids. After a long pause, another ghostly voice says: \"That didn't work. Let me try: I'll send the mortal screaming to the rafters.\"\n\nThe ensuing sound is suggestive of maggots, shrieking torments and fluttering dead things. Fortunately you do not see the manifestation that caused it. As far as the ghosts can tell, you are looking on at their best efforts to haunt you without batting an eyelid.\n\n\"Aren't you getting the least bit scared, mortal?\" asks a voice like a death-rattle.\n\n\"No,\" you say in feigned innocence. \"I'm quite enjoying the show, actually. Do go on.\"\n\nHaving been fooled into thinking you aren't frightened by them, the ghosts lose interest and return grumbling to their graves. Even so, you find it difficult to get any sleep with the thought of their corpses lying just under the mounds of earth. At dawn you emerge gratefully from the House of Gloom and retrieve your pack of belongings, eager for the final stage of your quest to commence.";

        Image = "images/filler1.png";

        Choices.clear();

        Controls = StandardControls();
    }

    int Continue(Character::Base &player) { return 40; }
};

class Story272 : public Story::Base
{
public:
    std::string PreText = "";

    Story272()
    {
        ID = 272;

        Image = "images/filler1.png";

        Choices.clear();

        Controls = StandardControls();
    }

    void Event(Character::Base &player)
    {
        PreText = "The onrushing figure looks just like a black rip in the air and makes no sound as he runs, but the impact when he hits you is like having a tree-branch swung into your midriff.";

        PreText += "\n\n";

        if (Character::VERIFY_SKILL(player, Skill::Type::AGILITY))
        {
            PreText += "[AGILITY] You deflect the blow and was not wounded.";
        }
        else
        {
            Character::GAIN_LIFE(player, -2);

            PreText += "You LOSE 2 Life Points.";
        }

        if (player.Life > 0)
        {
            PreText += "\n\nWhile you stagger back recovering your balance, your opponent sends the ball sailing against the high-score zone.";

            Character::SCORE(player, player.Cross, 2);
        }

        Text = PreText.c_str();
    }

    int Continue(Character::Base &player) { return 66; }
};

class Story273 : public Story::Base
{
public:
    std::string PreText = "";

    Story273()
    {
        ID = 273;

        Image = "images/filler1.png";

        Choices.clear();

        Controls = StandardControls();
    }

    void Event(Character::Base &player)
    {
        PreText = "As you race along the arena and up the steps of the pyramid, Necklace of Skulls conjures down a storm of blazing meteors.";

        PreText += "\n\n";

        auto DAMAGE = -3;

        if (Character::VERIFY_SKILL(player, Skill::Type::AGILITY))
        {
            DAMAGE = -1;

            PreText += "[AGILITY] ";
        }

        Character::GAIN_LIFE(player, DAMAGE);

        PreText += "You LOSE " + std::to_string(-DAMAGE) + " Life Point(s).";

        if (player.Life > 0)
        {
            if (Character::VERIFY_SKILL(player, Skill::Type::AGILITY))
            {
                PreText += "\n\n[AGILITY] You move too fast for him.";
            }

            PreText += "\n\nYou close with your foe for the final battle.";
        }

        Text = PreText.c_str();
    }

    int Continue(Character::Base &player)
    {
        if (Character::VERIFY_CODEWORD(player, Codeword::Type::VENUS))
        {
            return 240;
        }
        else
        {
            return 296;
        }
    }
};

class Story274 : public Story::Base
{
public:
    Story274()
    {
        ID = 274;

        Text = "The water between the two ships seethes, the a vast fanged maw bursts to the surface and long tentacles slash towards the sky. The pirates stare in terror, then scramble over one another in their mad haste to change course. You watch with a smile as they recede into the distance.\n\nThe trader is crouching in the bottom of the hull.\n\n\"A seas monster,\" he whimpers. \"It's come to kill us and seize all my goods!\"\n\nThe two boys are leaping up and down with whoops of joy, pulling faces at the fleeing pirates. One of them turns to his father. \"Oh, Dad,\" he says. \"It was just an illusion!\"";

        Image = "images/filler1.png";

        Choices.clear();

        Controls = StandardControls();
    }

    int Continue(Character::Base &player) { return 343; }
};

class Story275 : public Story::Base
{
public:
    Story275()
    {
        ID = 275;

        Text = "Apart from a few glowering looks, you are all but ignored by the people of Ashaka. Striding boldly up to a warrior in scarlet war paint, you ask directions to the market. He gives you a look like a bird studying a worm.\n\n\"Market?\" he sneers. \"There is no time for trade these days. We're preparing for war.\"\n\n\"War?\" you say naively. \"With whom?\"\n\n\"With everyone! Now the Great City's gone, all the upstart cities will start vying for dominance in the region.\"\n\n\"How sad and senseless,\" you sigh.\n\nHe spits into the dust. \"Don't be daft. Think about it. Now there'll have to be a new Great City. That's going to be us.\"\n\nShaking your head, you make your way through the streets until you find a furtive stallholder who is prepared to sell you a strip of salted meat for 1 cacao.";

        Image = "images/filler1.png";

        Choices.clear();
        Choices.push_back(Choice::Base("Head directly towards Shakalla", 298));
        Choices.push_back(Choice::Base("Detour to the coast and make your way via Tahil", 228));

        Shop = {{Item::Type::SALTED_MEAT, 1}};

        Controls = ShopControls();
    }
};

class Story276 : public Story::Base
{
public:
    Story276()
    {
        ID = 276;

        Text = "You get water from the bush by biting the tips off the hard sharp-tipped leaves and sucking the moisture out of them. It is barely enough to slake your thirst. You head on a cross the barren and blistered rocks, knowing that your ordeal will continue for many gruelling days yet.\n\nYou LOSE 2 Life Points.";

        Image = "images/filler1.png";

        Choices.clear();

        Controls = StandardControls();
    }

    void Event(Character::Base &player)
    {
        Character::GAIN_LIFE(player, -2);
    }

    int Continue(Character::Base &player) { return 152; }
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
        Choices.push_back(Choice::Base("You cannot spare the money and must bid the priest farewell and see about getting supplies for the trip", 93));

        Controls = StandardControls();
    }
};

class Story278 : public Story::Base
{
public:
    Story278()
    {
        ID = 278;

        Text = "On and on through the forest, the glowing figure leads you a madcap chase until your heart is pounding and your breath comes in raw sobs. Creepers hang across your path, but you dash them aside and race on. Sweat plasters your clothes to your skin. You catch your foot on a root and stumble, only to leap up again and stagger after your elusive quarry.\n\nAt last you emerge into a clearing beside a stream where you give a breathless cry of triumph. The girl has stopped at the bank of the stream. She stands with her back to you, SHAWL drawn around her golden body, her head bowed as if in passive acceptance that she can go no further.\n\nA final burst of effort carries you across the clearing, even though your legs feel like clay. The girl turns and favours you with a sweet smile. Then she breaks apart into hundreds of motes of dazzling light. Gasping for breath, you accidentally inhale one of the motes of light and feel an insectoid buzzing as you swallow it. A firefly. The 'girl' you were chasing was just a swarm of fireflies!\n\nSpluttering violently in a vain attempt to cough up the firefly you've just breathed in, you flail about and by blind luck your fingers close on the SHAWL. You feel someone is trying to tug it out of your hand, but the convulsive coughing makes you cling on tight.\n\nYou gained the codeword IGNIS.";

        Image = "images/filler1.png";

        Choices.clear();
        Choices.push_back(Choice::Base("Keep the SHAWL", 302, Choice::Type::GET_ITEM, Item::Type::SHAWL));
        Choices.push_back(Choice::Base("Leave the SHAWL", 324));

        Controls = StandardControls();
    }

    void Event(Character::Base &player)
    {
        Character::GET_CODEWORDS(player, {Codeword::Type::IGNIS});
    }
};

class Story279 : public Story::Base
{
public:
    Story279()
    {
        ID = 279;

        Text = "You must have wandered far to the south of your original route. But which way is north? The leaf canopy virtually covers the sky, giving you few clues about which way you should go.";

        Image = "images/filler1.png";

        Choices.clear();
        Choices.push_back(Choice::Base(Skill::WILDERNESS_LORE.Name, 441, Skill::Type::WILDERNESS_LORE));
        Choices.push_back(Choice::Base(Skill::CHARMS.Name, 75, Skill::Type::CHARMS));
        Choices.push_back(Choice::Base("Otherwise", 326));

        Controls = StandardControls();
    }
};

class Story280 : public Story::Base
{
public:
    Story280()
    {
        ID = 280;

        Text = "You set out in the early morning. A cool wind whips spindrift in your face and fills the taut white wing of the sail. The trader calls to his two young sons, who scramble along the vessel like monkeys making adjustments to the course. Soon the prow is chopping the sea aside as you steer out of sight of land with the sun at your backs.\n\n\"Shouldn't we keep the coast in sight?\" you ask.\n\n\"Not a bit of it!\" bellows the trader over the crash of the surf. \"The sun is enough to guide our way, for any seafarer with a pinch of salt in his blood.\"\n\n\"Father...\" says one of the sons, tugging the hem of his kilt.\n\n\"Not now, lad,\" mutters his father, scanning the western horizon as he moves the tiller.\n\n\"But, Father...\" insists the other boy.\n\nYou turn to see what they are looking at. A longship filled with fierce warriors is bearing down at you out of the north-east. As they get nearer, you see their black tunics and the turquoise beads that blaze at their wrists and throats. White face-paint gives them the appearance of hungry ghosts, and the raised spears make their intentions clear. Pirates!";

        Image = "images/pirates.png";

        Choices.clear();
        Choices.push_back(Choice::Base(Skill::SPELLS.Name, 274, Choice::Type::SKILL_ANY, Skill::Type::SPELLS, {Item::Type::MAGIC_WAND, Item::Type::JADE_SWORD}));
        Choices.push_back(Choice::Base(Skill::TARGETING.Name, 297, Skill::Type::TARGETING));
        Choices.push_back(Choice::Base("Otherwise", 320));

        Controls = StandardControls();
    }
};

class Story281 : public Story::Base
{
public:
    Story281()
    {
        ID = 281;

        Text = "You are surrounded by guards and any attempt to fight must be hampered by the throng of people crowding around the pit. As you lash out, the obsidian teeth of a guard's sword bit deeply into the flesh of your side.\n\n\"Be careful!\" commands the high priest. \"The Rain God will be angered if we damage his sacrifice!\"\n\nShoving one of the guards aside, you manage to struggle to the brink of the pit. The high priest steps back nervously as he sees the look of desperation in your eyes, raising his ritual knife to defend himself. The guards have regrouped and are spreading out to capture you.\n\nYou LOSE 3 Life Points.\n\nYou are losing blood from the wound you have taken.";

        Image = "images/filler1.png";

        Choices.clear();
        Choices.push_back(Choice::Base("Surrender", 327));
        Choices.push_back(Choice::Base("Fight on", 51));
        Choices.push_back(Choice::Base("Leap into the sacred well", 235));

        Controls = StandardControls();
    }

    void Event(Character::Base &player)
    {
        Character::GAIN_LIFE(player, -3);
    }
};

class Story282 : public Story::Base
{
public:
    Story282()
    {
        ID = 282;

        Text = "Your talent for stealth seems heightened in this otherworldly realm. You creep forward as noiselessly as a shadow. Gruesome slime coats the rough stone walls and the smell is almost unbearable, but you reach the chamber and stoop to inspect the item that caught your eye. It is a bowl of polished stone bearing the insignia of the Creator God, who gave life to all things, and you need no special senses to recognize the aura of divine magic. This can only be the fabled Chalice of Life in which the gods mixed the brew that spawned mankind.\n\nYou slip quietly back to the boat and gesture for the demons to convey you on.";

        Image = "images/filler1.png";

        Choices.clear();

        Controls = StandardControls();
    }

    void Event(Character::Base &player)
    {
        Character::GET_ITEMS(player, {Item::Type::CHALICE_OF_LIFE});
    }

    int Continue(Character::Base &player) { return 258; }
};

class Story283 : public Story::Base
{
public:
    Story283()
    {
        ID = 283;

        Text = "Circular marks are scored into the stone of the ledge. To anyone else they would mean nothing, but you realize that the slabs must be built to swivel on a central axis. Searching, your deft fingers soon find a concealed catch which unlocks the slab. With a grinding noise it revolves to reveal the darkened interior of the tomb.\n\nSomething moves within it. At first a sense of dread stirs the hairs on the nape of your neck, but it is no horrific walking corpse that emerges from the hole -- just a snake. You are almost relieved; at least this is a natural foe, although a deadly one. But then you see it is no ordinary snake. Its hood is enlarged to form translucent oval wings which flap slowly, carrying it on the warm updraught with a sinister gliding motion. Its iridescent scales make it resemble an obsidian idol, but there is no mistaking the living menace in those beady eyes and thin flickering tongue.";

        Image = "images/flying-snake.png";

        Choices.clear();
        Choices.push_back(Choice::Base("[SWORDPLAY] Try closing with the snake to fight it", 375, Choice::Type::SKILL_ANY, Skill::Type::SWORDPLAY, {Item::Type::SWORD, Item::Type::JADE_SWORD}));
        Choices.push_back(Choice::Base(Skill::UNARMED_COMBAT.Name, 375, Skill::Type::UNARMED_COMBAT));
        Choices.push_back(Choice::Base("[TARGETING] Shoot it", 352, Skill::Type::TARGETING));
        Choices.push_back(Choice::Base("[CHARMS] Cast a protective enchantment", 394, Skill::Type::CHARMS));
        Choices.push_back(Choice::Base("Use the MAN OF GOLD", 413, Item::Type::MAN_OF_GOLD));
        Choices.push_back(Choice::Base("Give up any hope of looting the tomb and climb back down to the canoe while you still can", 167));

        Controls = StandardControls();
    }
};

class Story284 : public Story::Base
{
public:
    Story284()
    {
        ID = 284;

        Text = "You bring some water back in your cupped hands. It is green with pond scum and smells like bile, so you are not surprised when the little fellow turns his long nose up at it. But then he clasps your arm and explains in a weak voice. \"It's not water I need, friend, but nice warm blood. Will you let me have drop or two of yours? If not, I fear I'm doomed to end my days here in this cold ditch.\"";

        Image = "images/filler1.png";

        Choices.clear();
        Choices.push_back(Choice::Base("Let him drink some of your blood", 395));
        Choices.push_back(Choice::Base("Shake free of his grip and continue on your way", 307));

        Controls = StandardControls();
    }
};

class Story285 : public Story::Base
{
public:
    Story285()
    {
        ID = 285;

        Text = "You must select an item from your possessions";

        Image = "images/filler1.png";

        std::vector<std::pair<Item::Type, int>> gifts = {
            {Item::Type::MAN_OF_GOLD, 192},
            {Item::Type::JADE_SWORD, 192},
            {Item::Type::GOLD_DIADEM, 192},
            {Item::Type::OWL, 55}};

        Choices.clear();
        Choices.push_back(Choice::Base("Choose", 78, gifts));
        Choices.push_back(Choice::Base("Try something else", 262));

        Controls = StandardControls();
    }
};

class Story286 : public Story::Base
{
public:
    Story286()
    {
        ID = 286;

        Text = "This is the so-called Death Canyon which prevents spirits returning form the deathlands by means of the underworld. The story goes that even the hero Jewelled Bird nearly fell when he tried jumping between the spires of rock, so you guess that such a feat would demand great agility. Dredging deeper into your memory you recall a legend of the dragon Kawak, a serpentine monster with a head at each end who is said to span this canyon, lying with one head on each side. The hero-twins Forethought and Afterthought returned from the Deathlands by walking into the open jaws of the dragon and making their way through its bowels. It goes without saying that such a route might not be so easy in reality as it was for such heroes of long ago. Kawak might just devour you!";

        Image = "images/filler1.png";

        Choices.clear();
        Choices.push_back(Choice::Base("Search for the two-headed dragon", 309));
        Choices.push_back(Choice::Base("Use a BLOWGUN to get across", 170, Item::Type::BLOWGUN));
        Choices.push_back(Choice::Base("Try jumping between the spires of rock", 147));

        Controls = StandardControls();
    }
};

class Story287 : public Story::Base
{
public:
    Story287()
    {
        ID = 287;

        Text = "All signs are that the woman is a victim of one of the parasitical monsters that people call nightcrawlers. These are disembodied heads that latch onto a human host, sinking tendrils deep into the flesh that allow them to control their victim like a puppet. By day they prefer to shield themselves from the sun, which no doubt explains the overturned pitcher on the woman's shoulder. After nightfall they detach from the host and go gliding about in search of blood to feed on.\n\nYou know that salt prevents the night-crawler from fixing onto its prey.";

        Image = "images/filler1.png";

        Controls = StandardControls();
    }

    int Continue(Character::Base &player)
    {
        if (Character::VERIFY_ITEMS(player, {Item::Type::PARCEL_OF_SALT}))
        {
            return 34;
        }
        else
        {
            return 57;
        }
    }
};

class Story288 : public Story::Base
{
public:
    Story288()
    {
        ID = 288;

        Text = "You stand over the fallen body. The eyes in the creature's ghastly face fix you with a last glare of fury, then roll up in their sockets. There is a dying rattle from the inhuman throat as it detaches itself from its host and rolls away, rotting like an overripe fruit the moment it enters the sunlight. You cover your nose with a gasp of disgust as you catch a whiff of the moulder stench that drifts up from it.\n\nYou kneel beside the poor woman who was forced to act as host to the disembodied head. Her neck was broken in the struggle. There is nothing you can do except give her a decent burial before heading grimly onwards.";

        Image = "images/filler1.png";

        Choices.clear();

        Controls = StandardControls();
    }

    int Continue(Character::Base &player) { return 417; }
};

class Story289 : public Story::Base
{
public:
    Story289()
    {
        ID = 289;

        Text = "In the mythology of your people, each of the cardinal points is associate with a colour. White is the colour of the north, the colour of mankind's ancient ancestors, a palace of lost memories. Red is the east, denoting birth and a return to fresh beginnings. Yellow is the southern route of misfortune and loss. And black is the west, where the Deathlands lie. That is the route you must follow.";

        Image = "images/filler1.png";

        Choices.clear();

        Controls = StandardControls();
    }

    int Continue(Character::Base &player) { return 219; }
};

class Story290 : public Story::Base
{
public:
    std::string PreText = "";

    Story290()
    {
        ID = 290;

        Image = "images/filler1.png";

        Choices.clear();

        Controls = StandardControls();
    }

    void Event(Character::Base &player)
    {
        PreText = "You press yourself into the angle where the floor meets the wall and slither forward like a snake. You pass right under the alcove where the first sentinel sits, but he never notices a thing. As you go past the second, he sniff the air suspiciously but says nothing. Beneath the alcove of the third, you accidentally scuff the stone flagging; the sentinel pricks up his ears and looks left and right along the passage, but he does not think to look down.\n\nYou are almost past the fourth alcove when your luck runs out. A speck of dust wafts up your nose and you give a loud sneeze. Instantly you are on your feet and running, but just as quickly the last sentinel makes a grab for you. He snatches away something you were carrying.";

        ToLose.clear();
        Limit = 0;

        if (player.Items.size() > 1)
        {
            for (auto i = 0; i < player.Items.size(); i++)
            {
                ToLose.push_back(player.Items[i]);
            }

            Limit = player.Items.size() - 1;
        }
        else if (player.Items.size() == 1)
        {
            Character::LOSE_ITEMS(player, {player.Items[0]});
        }
        else
        {
            PreText += "\n\nIt was a strip of flesh that he ripped off.\n\nYou LOSE 1 Life Point.";

            Character::GAIN_LIFE(player, -1);
        }

        Text = PreText.c_str();
    }
};

class Story291 : public Story::Base
{
public:
    std::string PreText = "";

    Story291()
    {
        ID = 291;

        Image = "images/filler1.png";

        Choices.clear();
        Choices.push_back(Choice::Base("Dodge back away from it", 245));
        Choices.push_back(Choice::Base("Make a headlong assault", 268));
        Choices.push_back(Choice::Base("Attempt to distract it with a feint", 419));

        Controls = StandardControls();
    }

    void Event(Character::Base &player)
    {
        PreText = "Two of the monster's head lash out together. You give a cry of pain as long curved fangs scythe a chunk out of your flesh.";

        auto DAMAGE = -3;
        auto BLOCKED = false;

        PreText += "\n\n";

        if (Character::VERIFY_SKILL_ANY(player, Skill::Type::SWORDPLAY, {Item::Type::SWORD, Item::Type::JADE_SWORD}))
        {
            BLOCKED = true;

            DAMAGE = -2;

            PreText += "[SWORDPLAY] ";
        }
        else if (Character::VERIFY_SKILL(player, Skill::Type::UNARMED_COMBAT))
        {
            BLOCKED = true;

            PreText += "[UNARMED COMBAT] ";

            DAMAGE = -2;
        }

        PreText += "You LOSE " + std::to_string(-DAMAGE) + " Life Points.";

        Character::GAIN_LIFE(player, DAMAGE);

        if (player.Life > 0)
        {
            if (BLOCKED)
            {
                PreText += "\n\nYou blocked one of the attacks.";
            }
        }

        Text = PreText.c_str();
    }
};

class Story292 : public Story::Base
{
public:
    Story292()
    {
        ID = 292;

        Text = "Your companion is led off across the courtyard towards a group of buildings. He looks back at you with sudden alarm, only to be jostled by the throng of dog-like courtiers surrounding him. \"Do not be afraid,\" you call after him. \"Courage and determination will win us through the trials to come.\"\n\nThe chief courtier picks at his teeth and murmurs in a mocking tone. \"By the look of him, he doesn't share your sterling qualities. Timidity and wretchedness seem more his mark.\"\n\nYou watch until the poor man is bustled inside, then round on the chief courtier. \"I demand to be taken to Necklace of Skulls.\"\n\n\"In good time,\" he chortles. \"Have I not invited you to be our guest first? After five nights you will be admitted to the sanctum of our master.\" He points a long hairy finger towards the pyramid that towers over the inner courtyard. The black colouring of the building makes it look like a sliver of night that remains to defy the daytime.\n\n\"Take me to my quarters, then,\" you tell him.\n\nThe assembled courtiers give a high howling laugh at this. \"Not so fast,\" titters the chief when he has recovered himself. \"First you have to choose your route to our compound.\"";

        Image = "images/filler1.png";

        Choices.clear();

        Controls = StandardControls();
    }

    int Continue(Character::Base &player) { return 315; }
};

class Story293 : public Story::Base
{
public:
    Story293()
    {
        ID = 293;

        Text = "You lick your lips nervously and venture another step closer to the burly hound. Its snarl becomes a roar as it throws itself at your throat.";

        Image = "images/filler1.png";

        Choices.clear();
        Choices.push_back(Choice::Base("Stand your ground and fight", 229));
        Choices.push_back(Choice::Base("Retreat to the colonnade at the mouth of the passage", 362));

        Controls = StandardControls();
    }
};

class Story294 : public Story::Base
{
public:
    std::string PreText = "";

    Story294()
    {
        ID = 294;

        Image = "images/filler1.png";

        Choices.clear();

        Controls = StandardControls();
    }

    void Event(Character::Base &player)
    {
        PreText = "The candle looks as though it will last for about an hour. Just as you are thinking this, a draft of cold air suddenly blows it out, plunging you into darkness.\n\nSilence hangs like a waiting presence in the air. The darkness dances in front of your eyes, causing your imagination to paint pictures of horror on the back of your mind. Your flesh creeps with unidentifiable fears.\n\nYou hear a noise that sets your heart pounding and every nerve shrieking. It was the sound of something dragging itself across the dry earthen floor. It stops beside you and you feel it reach out to stroke your leg: a thin dry hand with no flesh on it...\n\nAnd then you scream.\n\nAll through the night you are beset by gibbering phantoms that come prancing out of the darkness, running their unseen hands over your skin and whispering horrible things in your ears.\n\nYou LOSE 1 Life Point from this harrowing ordeal.";

        Character::GAIN_LIFE(player, -1);

        if (player.Life > 0)
        {
            PreText += "\n\nWhen the dawn arrives you stagger out on shaking legs to retrieve your pack of belongings. No matter what dangers you have to face now, you cannot conceive of anything more unpleasant than another night in the House of Gloom.";
        }

        Text = PreText.c_str();
    }

    int Continue(Character::Base &player) { return 40; }
};

class Story295 : public Story::Base
{
public:
    std::string PreText = "";

    Story295()
    {
        ID = 295;

        Image = "images/filler1.png";

        Choices.clear();

        Controls = StandardControls();
    }

    void Event(Character::Base &player)
    {
        PreText = "With a breathtaking lunge he catches up with the ball and strikes it a sweeping blow which sends it thudding up against the scoring zone.";

        if (Character::VERIFY_SKILL(player, Skill::Type::CHARMS))
        {
            PreText += "\n\nYou are unlucky and it catches you a hard blow on the side of the head. You LOSE 1 Life Point.";

            Character::GAIN_LIFE(player, -1);
        }

        if (player.Life > 0)
        {
            PreText += "\n\nWhen the dawn arrives you stagger out on shaking legs to retrieve your pack of belongings. No matter what dangers you have to face now, you cannot conceive of anything more unpleasant than another night in the House of Gloom.";
        }

        Text = PreText.c_str();
    }

    int Continue(Character::Base &player) { return 66; }
};

class Story296 : public Story::Base
{
public:
    std::string PreText = "";

    Story296()
    {
        ID = 296;

        Image = "images/necklace-of-skulls.png";

        Choices.clear();

        Controls = StandardControls();
    }

    void Event(Character::Base &player)
    {
        PreText = "Necklace of Skulls towers over you, his grotesque head looming like a pallid mushroom against the deep blue bowl of the heavens. His sword descends in a jagged sweep, and you throw yourself to one side. You hear it clash against the stone steps. At the same time as fighting you physically, he also uses his magic -- unleashing tendrils of poisonous gas from his throat and enchanting the skulls hanging at his chest so that they strain on their cords to snap at.";

        auto DAMAGE = -9;

        PreText += "\n\n";

        if (Character::VERIFY_SKILL_ANY(player, Skill::Type::SWORDPLAY, {Item::Type::SWORD, Item::Type::JADE_SWORD}))
        {
            DAMAGE += 2;

            PreText += "[SWORDPLAY] ";
        }

        if (Character::VERIFY_SKILL(player, Skill::Type::UNARMED_COMBAT))
        {
            PreText += "[UNARMED COMBAT] ";

            DAMAGE += 2;
        }

        if (Character::VERIFY_SKILL(player, Skill::Type::CHARMS))
        {
            PreText += "[CHARMS] ";

            DAMAGE += 2;
        }

        PreText += "You LOSE " + std::to_string(-DAMAGE) + " Life Points.";

        Character::GAIN_LIFE(player, DAMAGE);

        Text = PreText.c_str();
    }

    int Continue(Character::Base &player) { return 342; }
};

class Story297 : public Story::Base
{
public:
    Story297()
    {
        ID = 297;

        Text = "Your first dart hits a pirate right between his white-rimmed eyes and he slumps over the side with a groan. The others react to this with screeches of rage, paddling furiously to catch up with your own vessel. By the time the gap has been closed to ten paces, you have slain two more of them with your BLOWGUN and they are beginning to have second thoughts. When they see you slide another dart into the BLOWGUN, they throw up their hands in a gesture of surrender and go veering off towards the horizon -- no doubt in search of easier pickings.";

        Image = "images/filler1.png";

        Choices.clear();

        Controls = StandardControls();
    }

    int Continue(Character::Base &player) { return 343; }
};

class Story298 : public Story::Base
{
public:
    std::string PreText = "";

    Story298()
    {
        ID = 298;

        Text = NULL;

        Image = "images/filler1.png";

        Choices.clear();

        Bye = "Days turn to weeks. At last you catch sight of the town of Shakalla in the distance. Beyond it lies the great stony rim of the desert, crouching like a baleful predator at the edge of the world.";

        Controls = StandardControls();
    }

    void Event(Character::Base &player)
    {
        Choices.clear();

        PreText = "All along your route you find deserted cottages where people have abandoned their homes and farms for safer regions. You pass refugees on the mountain roads. Some tell you they are fleeing from brigands. Others fear the devils and werewolves that they believe will be unleashed from the western desert now the Great City has fallen.\n\nFood is hard to come by in the arid sierra.\n\nTravelling off the main paths to avoid the groups of marauding brigands, you are forced back on your own skills to find sustenance.";

        if (Character::VERIFY_ANY_SKILLS(player, {Skill::Type::TARGETING, Skill::Type::WILDERNESS_LORE}))
        {
            PreText += "\n\nYou were able to hunt for food.";

            Choices.push_back(Choice::Base("Continue", 321));
        }
        else if (Character::COUNT_ITEMS(player, {Item::Type::MAIZE_CAKES, Item::Type::PAPAYA, Item::Type::OWL, Item::Type::HAUNCH_OF_VENISON, Item::Type::SALTED_MEAT}))
        {
            PreText += "\n\nHaving no skills to hunt for food, you are forced to eat from your provisions.";

            Choices.push_back(Choice::Base("Eat from your provisions", 321, Choice::Type::EAT, {Item::Type::MAIZE_CAKES, Item::Type::PAPAYA, Item::Type::OWL, Item::Type::HAUNCH_OF_VENISON, Item::Type::SALTED_MEAT}, 2));
        }
        else
        {
            Choices.push_back(Choice::Base("Continue (LOSE 2 Life Points)", 321, Choice::Type::LIFE, -2));
        }

        Text = PreText.c_str();
    }
};

class Story299 : public Story::Base
{
public:
    Story299()
    {
        ID = 299;

        Text = "You break the stem of the cactus and it oozes a clear sweet-smelling fluid. You taste a little, then suck at it until you have drained all you can. It is barely enough. You stumble on across the lifeless wilderness of stone and sand, thirst burning a throbbing gulf in your throat.\n\nYou LOSE 4 Life Points.";

        Image = "images/filler1.png";

        Choices.clear();

        Controls = StandardControls();
    }

    void Event(Character::Base &player)
    {
        Character::GAIN_LIFE(player, -4);
    }

    int Continue(Character::Base &player) { return 152; }
};

class Story300 : public Story::Base
{
public:
    Story300()
    {
        ID = 300;

        Text = "Two days' sailing brings you to Tahil, a busy trading settlement on the far coast. The others squint warily as they bring the vessel in to the harbour. You can see at once there is trouble here. Instead of the stacks of trade goods that would normally be piled up along the quayside at a port like this, there are milling crowds of refugees carrying everything they own on their backs.\n\nAs you tie up at the quay, a man whose elegant clothing marks him as a lord of the Great City comes striding towards you. \"I am commandeering your vessel,\" he says in a tone that brooks no disagreement.\n\nWithout waiting for a reply, he turns and beckons his wife and children to join him. A couple of servants scurry along behind them, struggling under the weight of the family's possessions.";

        Image = "images/maya-noble.png";

        Choices.clear();
        Choices.push_back(Choice::Base("[ETIQUETTE] Prevent his appropriation of the vessel", 159, Skill::Type::ETIQUETTE));
        Choices.push_back(Choice::Base(Skill::CUNNING.Name, 183, Skill::Type::CUNNING));
        Choices.push_back(Choice::Base(Skill::SWORDPLAY.Name, 206, Choice::Type::SKILL_ANY, Skill::Type::SWORDPLAY, {Item::Type::SWORD, Item::Type::JADE_SWORD}));
        Choices.push_back(Choice::Base("You are not bothered about him taking the vessel, you can just set out towards Shakalla", 85));

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
        if (player.DONATION > 4)
        {
            return 323;
        }
        else
        {
            return 346;
        }
    }
};

class Story302 : public Story::Base
{
public:
    Story302()
    {
        ID = 302;

        Text = "The effort of trying to regurgitate the firefly leaves your eyes watering, but to no avail. Before you can blink away the tears, you realize someone is trying to pull the SHAWL out of your grip. You have the impression of someone very tall and unbelievably thin, like a human stick insect. The face is as pale as sap, and the naked skin exudes a sweet smell like tree bark and honey.\n\nYou wipe your eyes and look again. At first you think the figure is gone, but then there is a flicker of movement at the edge of your vision. Whirling, you catch a brief glimpse of two or three thin creatures. They dart away like pale green fishes to hang just behind your line of sight. By standing still you get an uncertain and unfocused view of them out of the corner of your eye. It dawns on you that there are many of them clustered noiselessly around the clearing, transient as sunbeams. They are the stabai -- the creatures of the wooded dells, who ply their magic to the discomfort of lone travellers like yourself.\n\n\"You have our SHAWL,\" buzzes a soft forlorn voice in your ear. \"Won't you return it?\"";

        Image = "images/filler1.png";

        Choices.clear();
        Choices.push_back(Choice::Base(Skill::FOLKLORE.Name, 255, Skill::Type::FOLKLORE));
        Choices.push_back(Choice::Base("Return the SHAWL", 347, Item::Type::SHAWL));
        Choices.push_back(Choice::Base("Keep the SHAWL", 369, Item::Type::SHAWL));

        Controls = StandardControls();
    }
};

class Story303 : public Story::Base
{
public:
    Story303()
    {
        ID = 303;

        Text = "The sun rises again, flooding the sands with red-gold light. You are contemplating the need for shelter when, cresting a dune, you catch sight of it at long last -- the wizard's palace. Double ramparts surround a central courtyard where a high black pyramid rises like a blotch of midnight. The palace lies just ahead across a stretch of brown-gold sand. The dawn light makes it seem to shimmer like a mirage in the deep blue shadows between the dunes, but you know it is real.\n\nDouble doors swing open in the wall as you approach. Confronting you is a horde of men in ragged animal skins. Their long thin faces and downcast smiles give them a canine appearance. All of them bear stone axes which they lift when you walk through the palace gate -- not a gesture of impending violence, but just to warn you where you stand.\n\n\"I have come,\" you say, \"to see Necklace of Skulls.\"\n\nOne of the men gives a bark of laughter. \"It's not as easy as that. Do you think our master sees every stray mongrel who wanders to his door? First you will have to pass five nights among us, his faithful courtiers. And before that you'll face another test: deciding which route to take to the inner courtyard.\"";

        Image = "images/filler1.png";

        Choices.clear();

        Controls = StandardControls();
    }

    int Continue(Character::Base &player) { return 315; }
};

class Story304 : public Story::Base
{
public:
    std::string PreText = "";

    Story304()
    {
        ID = 304;

        Image = "images/filler1.png";

        Choices.clear();

        Controls = StandardControls();
    }

    void Event(Character::Base &player)
    {
        PreText = "From what your friend the old soothsayer told you, it is unwise to speak once you enter the underworld. You had better cast your spells now, while you still can.\n\n\"Allow me a moment to compose myself,\" you say to the high priest.\n\n\"It is irregular...\"\n\n\"I am about to meet a god,\" you point out.\n\nStepping away a few paces, you murmur the words of an incantation which renders your body as buoyant as wood. You gaze down into the depths of the well. At least now you can be sure of not drowning -- but if this is truly the entrance to the underworld, there will be other dangers which you cannot yet even guess at.\n\nYou turn to the assembled crowd and announce, \"I am ready.\"\n\nYou are led to a shrine at the western edge of the hole. From here, a steep flight of steps descends towards a platform covered with the hieratic glyphs of the afterlife. A vest of golden plaques is fastened across your chest and the high priest places a tall helmet of gold and copper on your head. The burden of so much metal makes you stoop. These artifacts are beyond price, since gold is not found in this part of the world. It is a lavish offering to the Rain God, but it also serves a secondary purpose: the great weight ensures you will be carried deep under the water.\n\nSeveral junior priests come forward with dishes of blue dye, which they use to paint spirals across your face and limbs. \"Thus you are consecrated to the Rain God,\" they explain. \"Go now into the other world, and carry our plea for rain to refresh the arid fields.\"";

        if (Character::VERIFY_ITEMS(player, {Item::Type::JADE_BEAD}))
        {
            PreText += "\n\nYou lodged the JADE BEAD under your tongue as the spirit advised you.";
        }

        PreText += "\n\nThen you descend to the platform overlooking the well and prepare yourself for the most uncanny voyage anyone could every attempt: a leap into the underworld.";

        Text = PreText.c_str();
    }

    int Continue(Character::Base &player) { return 411; }
};

class Story305 : public Story::Base
{
public:
    Story305()
    {
        ID = 305;

        Text = "Which of these items will you use?";

        Image = "images/filler1.png";

        Choices.clear();
        Choices.push_back(Choice::Base("Use the MAN OF GOLD", 351, Item::Type::MAN_OF_GOLD));
        Choices.push_back(Choice::Base("... the PARCEL OF SALT", 372, Item::Type::PARCEL_OF_SALT));
        Choices.push_back(Choice::Base("... the FIREBRAND", 393, Item::Type::FIREBRAND));
        Choices.push_back(Choice::Base("Otherwise, you will have to fight", 328));

        Controls = StandardControls();
    }
};

class Story306 : public Story::Base
{
public:
    Story306()
    {
        ID = 306;

        Text = "The MAN OF GOLD comes to life in the warmth of your hands. You set him down on the ledge and he surveys the sealed tomb with a smile on his tiny sculpted features. You have no need to tell him what you want him to do. Striding forward, he batters at the slab with thunderous blows. You are amazed to see chips of stone fly away as he gradually tunnels a hole large enough for you to squeeze through. Then, his task completed, he turns and dives off into the river below.\n\nA movement from inside the tomb makes you jump. Out of the hole slithers a cobra. It is no ordinary cobra: its hood is vastly extended to form translucent oval wings which beat slowly, carrying it through the air with a sinister gliding motion. With its iridescent scales it seems like a polished stone idol, but there is no mistaking the living menace in the glittering eyes and forked tongue.";

        Image = "images/flying-snake.png";

        Choices.clear();
        Choices.push_back(Choice::Base("Retreat back down to the canoe", 167));
        Choices.push_back(Choice::Base("[SWORDPLAY] Close with the snake and fight it", 375, Choice::Type::SKILL_ANY, Skill::Type::SWORDPLAY, {Item::Type::SWORD, Item::Type::JADE_SWORD}));
        Choices.push_back(Choice::Base(Skill::UNARMED_COMBAT.Name, 375, Skill::Type::UNARMED_COMBAT));
        Choices.push_back(Choice::Base("[TARGETING] Shoot it", 352, Skill::Type::TARGETING));
        Choices.push_back(Choice::Base("[SPELLS] Cast a protective enchantment", 394, Choice::Type::SKILL_ANY, Skill::Type::SPELLS, {Item::Type::MAGIC_WAND, Item::Type::JADE_SWORD}));

        Controls = StandardControls();
    }

    void Event(Character::Base &player)
    {
        Character::LOSE_ITEMS(player, {Item::Type::MAN_OF_GOLD});
    }
};

class Story307 : public Story::Base
{
public:
    Story307()
    {
        ID = 307;

        Text = "A bundle lies across the path ahead. Quickening your pace, you are startled when the 'bundle' suddenly raises its head. It is a dwarf with withered legs but very broad shoulders wrapped in a long black mantle. His large ears and upturned nose give him a grotesque appearance, and you are on the point of hurrying past when he raises a long imploring arm.\n\n\"Please help me,\" he begs. \"I'm too weak to go on, and I'm terribly thirsty...\"";

        Image = "images/filler1.png";

        Choices.clear();
        Choices.push_back(Choice::Base("Go back to the river to fetch him some water", 330));
        Choices.push_back(Choice::Base("Gash your hand and let him drink some of your blood", 353));
        Choices.push_back(Choice::Base("Step over him and continue along the path without stopping", 373));

        Controls = StandardControls();
    }
};

class Story308 : public Story::Base
{
public:
    Story308()
    {
        ID = 308;

        Text = "You discover that King Cloud Jaguar is sitting with his courtiers in the steam bath at the rear of the palace. Leaving your clothes and belongings in a bundle behind a wall, you mingle in among the servants and pick up a jar of water. Clad in only loin-cloth, there is nothing to draw attention to you when you enter the bath-house with downcast eyes and pour water onto the sizzling stones of the hearth.\n\nThe King and the nobles of his court sit naked on the benches of the bath-house, sweltering in the wreathes of herb-scented steam. \"Will you be communing with your late father tonight, your Majesty?\" you hear one of the nobles asking.\n\n\"Of course!\" says the King gruffly. \"I must discover the import of recent events. The collapse of the Great City will have far-reaching consequences.\"\n\n\"The astrologers have read the signs,\" puts in the King's vizier. \"The most auspicious time is the hour just before dawn. We shall ascend the pyramid and speak our questions into the spirit tube which leads to King Sky Shield's tomb. Perhaps you would like to accompany us, Lord Smoke Shell?\"\n\nThe noble who spoke first looks up with sudden alarm. \"Ah... er, that's most gracious, but I need my sleep if I'm to cope with all the excitement tomorrow. I'm not as young as I once was.\"\n\n\"Nor as brave,\" says the King pointedly, provoking a gust of laughter at Smoke Shell's expense. You leave the bath-house and retrieve your belongings.";

        Image = "images/filler1.png";

        Choices.clear();
        Choices.push_back(Choice::Base("Seek lodging if you have some money", 101, Choice::Type::MONEY, 1));
        Choices.push_back(Choice::Base("Continue on your journey westwards", 8));
        Choices.push_back(Choice::Base("Continue north to the coast", 30));

        Controls = StandardControls();
    }
};

class Story309 : public Story::Base
{
public:
    Story309()
    {
        ID = 309;

        Text = "You walk along the side of the canyon in search of the dragon Kawak. The sky slowly acquires a sullen red glare like the inside of a kiln. Through the clouds of gritty yellow vapour rising out of the depths, you begin to discern a rock outcropping just ahead. Then you take a step or two closer and you realize it is not a pile of boulders that you see resting at the lip of the chasm. It is a gigantic head.\n\nKawak's snout reminds you of the temple pyramids of your homeland, with its size and bright patterning of carmine and bone-white. His mouth stands open like an ominous gateway, tongue leading up like a temple's stairway to where his eyes burn darkly under brows like shields of stone. You can see the immense curve of his body, as broad as a city plaza, arcing off into the distance towards the other side of the canyon. His claws resemble the prows of ocean-going canoes hanging over the edge of the precipice.\n\n\"You wish to cross. To enter my mouth and pass into the world beyond.\" His voice is the sound of birth and death, the dull titanic roar that is more felt than heard. \"You must pay for your passage with a JADE BEAD.";

        Image = "images/filler1.png";

        Choices.clear();
        Choices.push_back(Choice::Base("Let Kawak have the JADE BEAD", 33, Choice::Type::GIVE_ITEM, Item::Type::JADE_BEAD));
        Choices.push_back(Choice::Base("Employ the MAN OF GOLD", 79, Item::Type::MAN_OF_GOLD));
        Choices.push_back(Choice::Base("Use a BLOWGUN", 170, Item::Type::BLOWGUN));
        Choices.push_back(Choice::Base("Try crossing via the line of stepping-stones formed by the tips of the rock spires", 147));

        Controls = StandardControls();
    }
};

class Story310 : public Story::Base
{
public:
    Story310()
    {
        ID = 310;

        Text = "This situation reminds you unpleasantly of what the fenman told you about nightcrawlers. You find that you cannot even consider what might be hidden under the overturned pitcher without a shudder of horror. The fenman also said that salt will stop a nightcrawler from attaching itself to a victim.";

        Image = "images/filler1.png";

        Choices.clear();

        Controls = StandardControls();
    }

    void Event(Character::Base &player)
    {
        Character::REMOVE_CODEWORD(player, Codeword::Type::CALABASH);
    }

    int Continue(Character::Base &player)
    {
        if (Character::VERIFY_ITEMS(player, {Item::Type::PARCEL_OF_SALT}))
        {
            return 34;
        }
        else
        {
            return 57;
        }
    }
};

class Story311 : public Story::Base
{
public:
    std::string PreText = "";

    Story311()
    {
        ID = 311;

        Image = "images/flying-head.png";

        Choices.clear();

        Controls = StandardControls();
    }

    void Event(Character::Base &player)
    {
        Type = Story::Type::NORMAL;

        PreText = "You roll forward, carrying yourself and your hideous foe to the ground. It emits a gurgle of vaunting glee as it presses the stump of its neck against your shoulder, trying to drive its vampiric tendrils into your flesh and make you its willing host.";

        if (!Character::VERIFY_SKILL(player, Skill::Type::CHARMS) && !Character::VERIFY_CODEWORD(player, Codeword::Type::SALVATION))
        {
            PreText += "\n\nYou have no defence against the creature's malign magic and you are doomed to become its next victim.";

            Type = Story::Type::DOOM;
        }

        Text = PreText.c_str();
    }

    int Continue(Character::Base &player) { return 357; }
};

class Story312 : public Story::Base
{
public:
    Story312()
    {
        ID = 312;

        Text = "You set your wand down on the ground in the middle of the crossroads and spin it, meanwhile uttering the words of a divining spell. It comes to rest pointing along the black path. That is the route you must take to reach the Deathlands.";

        Image = "images/filler1.png";

        Choices.clear();

        Controls = StandardControls();
    }

    int Continue(Character::Base &player) { return 219; }
};

class Story313 : public Story::Base
{
public:
    Story313()
    {
        ID = 313;

        Text = "You approach the first of the sentinels. Seeing you, he leans forward on his granite throne and taps your shoulder with a sceptre carved from a human thighbone. \"Where are your manners?\" he thunders. His voice sounds like a shriek of an eagle in the instant of seizing its prey.\n\nWhat name will you greet him by?";

        Image = "images/filler1.png";

        Choices.clear();
        Choices.push_back(Choice::Base("'Lord Blood'", 401));
        Choices.push_back(Choice::Base("'Thunderbolt Laughter'", 380));
        Choices.push_back(Choice::Base("'Grandfather of Darkness'", 359));
        Choices.push_back(Choice::Base("'Lord Skull'", 14));

        Controls = StandardControls();
    }
};

class Story314 : public Story::Base
{
public:
    Story314()
    {
        ID = 314;

        Text = "Again you avoid the monster's bites by a hair's breadth. Spitting with fury at its failure to trap you, it slithers forward with all four necks extended.\n\nDecide your next move.";

        Image = "images/filler1.png";

        Choices.clear();
        Choices.push_back(Choice::Base("Continue to jump away from the hydra", 360));
        Choices.push_back(Choice::Base("Rush in to strike a blow", 381));
        Choices.push_back(Choice::Base("Try a feint", 419));

        Controls = StandardControls();
    }
};

class Story315 : public Story::Base
{
public:
    Story315()
    {
        ID = 315;

        Text = "You are taken to the back wall of the courtyard, where colonnade gives respite from the intense sun. Beneath a frieze patterned like a rattlesnake's skin, four archways lead on from here to the next courtyard of the palace. The courtiers give you wide grins and invite you to make a choice.\n\nYou look into the first archway. A short flight of steps leads up to the edge of a pit about two metres across. The purpose of the steps is apparently to ensure that you cannot take a running jump. The bottom of the pit is filled with smoking coals.\n\nBeyond the next archway is a tunnel blocked by an artful tangle of wooden beams, some of which seem to be shoring up the walls. \"The trick there is to remove the right combination of beams in the right order,\" says the chief courtier, leaning over your shoulder. \"You want to clear enough space to get past without causing the tunnel to collapse.\"\n\nThe third route is a passage with no obstacles -- just a triangular vault leading through to the next courtyard. then you notice the sigils inscribed along the corbels of the vault: sigils indicating calamity and catastrophe. Is the vault designed to cave in when someone walks along it?\n\nThe passage beyond the last archway is guarded by a large pallid hound with narrow pink eyes. It greets you with a threatening growl as you poke your head around the corner, but makes no move to attack. \"My second cousin twice removed,\" says the chief courtier in your ear. \"Nasty temper. His bite's worse than his bark, of course.\"\n\nIt is time for you to decide.";

        Image = "images/filler1.png";

        Choices.clear();
        Choices.push_back(Choice::Base("Cross the pit", 338));
        Choices.push_back(Choice::Base("Attempt to clear the blocked tunnel", 361));
        Choices.push_back(Choice::Base("Walk along the unguarded tunnel", 382));
        Choices.push_back(Choice::Base("Brave the albino hound", 420));

        Controls = StandardControls();
    }
};

class Story316 : public Story::Base
{
public:
    std::string PreText = "";

    Story316()
    {
        ID = 316;

        Image = "images/filler1.png";

        Choices.clear();

        Controls = StandardControls();
    }

    void Event(Character::Base &player)
    {
        PreText = "Your dart tears into the hound's flank, leaving a raw bloody gash. It whines and shrinks back, only to leap forward without warning when you move to get past it along the tunnel. You feel a stab of pain as its teeth tear a hunk of flesh out of your thigh, but you stagger on regardless.\n\nYou LOSE 1 Life Point.";

        Character::GAIN_LIFE(player, -1);

        if (player.Life > 0)
        {
            PreText += "\n\nYou emerge into the inner courtyard to find the courtiers already there waiting for you. \"You're a blowgunner, eh?\" says the chief courtier. \"Well, I'm afraid you'll find the creatures who serve Necklace of Skulls are less easy to intimidate than your average deer.\"";
        }

        Text = PreText.c_str();
    }

    int Continue(Character::Base &player) { return 431; }
};

class Story317 : public Story::Base
{
public:
    Story317()
    {
        ID = 317;

        Text = "You squint in the shimmering glare of the sun, but your eyes cannot make out any shape within the black void of the shrine entrance. The soot-coloured pillars give its darkened interior the look of a fleshless mouth. Again Necklace of Skull's voice rolls along the avenue, each syllable driving like a grave-cool gust of wind through the blistering desert heat. \"Your brother came here before you.\"\n\nRed rage seethes in your heart. \"That's right, you --\"\n\nThe sorcerer's words continue, unperturbed by your outburst. \"He played the ritual ball contest and he lost. His life was forfeit. Now you will play for the same stakes. Behold your antagonists.\" Two long rivulets of shadow flow out of the shrine and down the pyramid steps, looking like spreading pools of black blood against the ebon stone. Reaching the bottom, they rise up in obscenely palpitating columns which gradually take solid form. Human form. Confronting you now are your opponents in the ball contest: two creatures of living shadow fashioned by the sorcerer's magic.";

        Image = "images/filler1.png";

        Choices.clear();

        Controls = StandardControls();
    }

    void Event(Character::Base &player)
    {
        player.RitualBallStarted = true;
    }

    int Continue(Character::Base &player)
    {
        if (Character::VERIFY_ITEMS(player, {Item::Type::BROTHERS_SKULL}))
        {
            return 340;
        }
        else if (Character::VERIFY_CODEWORD(player, Codeword::Type::ANGEL))
        {
            return 363;
        }
        else
        {
            return 384;
        }
    }
};

class Story318 : public Story::Base
{
public:
    Story318()
    {
        ID = 318;

        Text = "You bear down on the enemy defensive player. Glancing back you see that the far shadow man has reached the far end of the arena, where your partner has managed to intercept the ball and get possession. You shout for him to bounce it along the wall towards you.";

        Image = "images/filler1.png";

        Choices.clear();

        Controls = StandardControls();
    }

    int Continue(Character::Base &player)
    {
        if (Character::VERIFY_CODEWORD(player, Codeword::Type::SHADE))
        {
            return 385;
        }
        else
        {
            return 405;
        }
    }
};

class Story319 : public Story::Base
{
public:
    std::string PreText = "";

    Story319()
    {
        ID = 319;

        Image = "images/filler1.png";

        Choices.clear();

        Controls = StandardControls();
    }

    void Event(Character::Base &player)
    {
        PreText = "You manage to get the firebrand alight. It gives a little warmth, but not much. You spend a miserable night shivering in the icy draughts blowing through the House of Cold.";

        PreText += "\n\n";

        if (Character::VERIFY_SKILL(player, Skill::Type::WILDERNESS_LORE))
        {
            PreText += "[WILDERNESS LORE] Your hardiness inures you to the extreme cold.";
        }
        else
        {
            PreText += "You LOSE 1 Life Point.";

            Character::GAIN_LIFE(player, -1);
        }

        Text = PreText.c_str();
    }

    int Continue(Character::Base &player) { return 212; }
};

class Story320 : public Story::Base
{
public:
    std::string PreText = "";

    Story320()
    {
        ID = 320;

        Image = "images/filler1.png";

        Choices.clear();

        Controls = StandardControls();
    }

    void Event(Character::Base &player)
    {
        PreText = "The pirates' vessel bumps against yours and they come swarming over the side like ants. You cannot understand their yammering war-cries, for they are not of your people but come from a land beyond the northern sea. Their white-rimmed eyes hold a look that means the same in any language, though. It is the look of fury and hate.\n\nYou fight with desperate strength knowing that you are fighting not just for your own life but also for the trader and his young sons.";

        auto DAMAGE = -9;

        PreText += "\n\n";

        if (Character::VERIFY_SKILL_ANY(player, Skill::Type::SWORDPLAY, {Item::Type::SWORD, Item::Type::JADE_SWORD}))
        {
            DAMAGE = -3;

            PreText += "[SWORDPLAY] ";
        }
        else if (Character::VERIFY_SKILL(player, Skill::Type::UNARMED_COMBAT))
        {
            PreText += "[UNARMED COMBAT] ";

            DAMAGE = -5;
        }

        PreText += "You LOSE " + std::to_string(-DAMAGE) + " Life Points.";

        Character::GAIN_LIFE(player, DAMAGE);

        if (player.Life > 0)
        {
            PreText += "\n\nYou finally force the pirates to retreat to their own vessel and sail off.";
        }

        Text = PreText.c_str();
    }

    int Continue(Character::Base &player) { return 343; }
};

class Story321 : public Story::Base
{
public:
    Story321()
    {
        ID = 321;

        Text = "Shakalla is a walled town whose hard sun-baked streets are the colour of hot ash. At this time of day, the place is deserted. Those who have not fled in fear have retreated to the cool interiors of their houses, seeking refuge from the midday sun. You see a few faces peering from the narrow doorways as you go past. A dog lies stretched in the shade of a shop's awning, panting with the heat.";

        Image = "images/filler1.png";

        Choices.clear();
        Choices.push_back(Choice::Base("Enter the shop", 344));
        Choices.push_back(Choice::Base("Carry on to the west gate of the town", 325));

        Controls = StandardControls();
    }
};

class Story322 : public Story::Base
{
public:
    Story322()
    {
        ID = 322;

        Text = "You become weaker and weaker. Finally you collapse and lie stretched out on the hard sun-blistered ground, too weak to rise. You feel like one who has been chosen for sacrifice to the gods, spread-eagled and helpless. Your eyes are stinging. You stare up at the sky, which first blackens until it is like ink, then explodes in a hazy burst of light. Now you are weightless, dropping down an endless tunnel that leads into the bowels of the earth...\n\nAnd so you die, far out in the cruel desert with only the vultures to witness your last agonized moments. Perhaps you will be reunited with your brother in the afterlife -- but only the gods can say.";

        Image = "images/filler1.png";

        Type = Story::Type::DOOM;

        Choices.clear();

        Controls = StandardControls();
    }
};

class Story323 : public Story::Base
{
public:
    Story323()
    {
        ID = 323;

        Text = "\"A generous donation.\" The high priest puts the money into his belt-pouch. \"I shall see that a ritual of benediction is performed for you at the temple tomorrow morning. You will carry the blessing of the god.\"\n\nThe blessing of the War God means that you can ignore any one injury taken at any point of your choice during the adventure. Remember that it only works once.\n\n\"How about a spot of ball practice?\" suggests the high priest.";

        Image = "images/filler1.png";

        Choices.clear();
        Choices.push_back(Choice::Base("Join him for a practice bout in the arena", 368));
        Choices.push_back(Choice::Base("You had better get on with the rest of your preparations for the journey", 93));

        Controls = StandardControls();
    }

    void Event(Character::Base &player)
    {
        player.IsBlessed = true;
    }
};

class Story324 : public Story::Base
{
public:
    Story324()
    {
        ID = 324;

        Text = "You gaze around. All directions present you with the same vista: trees stand like stone sentinels draped in moss-coloured gloom. Great nets of creepers are strung between their heavy boughs. There are not even any sounds of wildlife to disturb the hoary solitude of the ancient woods. You are thoroughly lost.";

        Image = "images/filler1.png";

        Choices.clear();
        Choices.push_back(Choice::Base("Continue", 118));

        Controls = StandardControls();
    }

    void Event(Character::Base &player)
    {
        if (player.Life < player.MAX_LIFE_LIMIT)
        {
            if (Character::VERIFY_SKILL(player, Skill::Type::WILDERNESS_LORE))
            {
                Choices[0].Destination = 72;
            }
            else
            {
                Choices[0].Destination = 95;
            }
        }
        else
        {
            Choices[0].Destination = 118;
        }
    }
};

class Story325 : public Story::Base
{
public:
    Story325()
    {
        ID = 325;

        Text = "A low tunnel in the wall around the town forms the west gate. Crouching as you make your way along the tunnel, you notice rough scrawlings in the stone. One shows a man with the tail and claws of a scorpion: another is a four-headed dragon.\n\nTwo guards armed with spear stand at the far end of the tunnel, staring nervously out across the desert. As they hear you come up, they glance at you and one says, \"Here's a traveller who isn't afraid to take the Gate of Exiles!\"\n\nBeyond, in the bright sunshine, lies the desert you must cross. The bleak stony landscape of crags and dusty gullies stretches off into a haze of heat along the horizon. A path leads from the gate, but it peters out beside a large boulder ten paces away.";

        Image = "images/gate-of-exiles.png";

        Choices.clear();
        Choices.push_back(Choice::Base("Question the guards about the markings on the tunnel walls", 367));
        Choices.push_back(Choice::Base("... or the nearby boulder", 388));
        Choices.push_back(Choice::Base("Set out in the desert", 407));

        Controls = StandardControls();
    }
};

class Story326 : public Story::Base
{
public:
    Story326()
    {
        ID = 326;

        Text = "You follow a game trail that forks beside a high moss-caked boulder. Sunlight slants down through chinks in the foliage, teasing you with inadequate clues as to your bearing.";

        Image = "images/filler1.png";

        Choices.clear();
        Choices.push_back(Choice::Base("Go left", 412));
        Choices.push_back(Choice::Base("Take the right-hand fork", 348));

        Controls = StandardControls();
    }
};

class Story327 : public Story::Base
{
public:
    std::string PreText = "";

    Story327()
    {
        ID = 327;

        Image = "images/filler1.png";

        Choices.clear();
        Choices.push_back(Choice::Base(Skill::SPELLS.Name, 411, Choice::Type::SKILL_ANY, Skill::Type::SPELLS, {Item::Type::MAGIC_WAND, Item::Type::JADE_SWORD}));
        Choices.push_back(Choice::Base(Skill::CHARMS.Name, 189, Skill::Type::CHARMS));
        Choices.push_back(Choice::Base(Skill::CUNNING.Name, 371, Skill::Type::CUNNING));
        Choices.push_back(Choice::Base(Skill::SEAFARING.Name, 392, Skill::Type::SEAFARING));
        Choices.push_back(Choice::Base(Item::Descriptions[Item::Type::BLOWGUN], 5, Item::Type::BLOWGUN));
        Choices.push_back(Choice::Base("Otherwise", 28));

        Controls = StandardControls();
    }

    void Event(Character::Base &player)
    {
        PreText = "You are led to a shrine at the western edge of the hole. From here, a steep flight of steps descends towards a platform covered with the hieratic glyphs of the afterlife. You will soon walk down those steps and, after reaching the platform, plunge far down into the dark waters below.\n\nA vest of golden plaques is fastened across your chest and the high priest places a tall helmet of gold and copper on your head. The burden of so much metal makes you stoop. These artifacts are beyond price since gold is not found in this part of the world. It is a lavish offering to the Rain God, but it also serves a secondary purpose: the great weight ensures you will be carried deep under the water.\n\nSeveral junior priests come forward with dishes of blue dye, which they use to paint spirals across your face and limbs. \"Thus you are consecrated to the Rain God,\" they explain. \"Go now into the other world, and carry our plea for rain to refresh the arid fields!\"";

        if (Character::VERIFY_ITEMS(player, {Item::Type::JADE_BEAD}))
        {
            PreText += "\n\nYou remember to slip the JADE BEAD under your tongue as advised.";
        }

        PreText += "\n\nThen you descend to the platform overlooking the well and prepare yourself for the most uncanny voyage anyone could ever attempt: a leap into the underworld.";

        Text = PreText.c_str();
    }
};

class Story328 : public Story::Base
{
public:
    std::string PreText = "";

    Story328()
    {
        ID = 328;

        Image = "images/filler1.png";

        Choices.clear();

        Controls = StandardControls();
    }

    void Event(Character::Base &player)
    {
        Type = Story::Type::NORMAL;

        PreText = "You cannot suppress a scream as their wet stubby fingers clutch at you. It is like a scene from your worst nightmares. Striking out blindly, you stumble along the tunnel. You feel their shells crack under your frantic blows, but then a slimy hand presses over your face and you reel under a hail of punches. Ichor spurts into your eyes as you drive a deep blow hard into your eyes as you drive a deep blow hard into one of the creatures, and its eerie keening wail evokes a feeling of unendurable horror.";

        auto DAMAGE = 0;

        PreText += "\n\n";

        if (Character::VERIFY_SKILL_ANY(player, Skill::Type::SWORDPLAY, {Item::Type::SWORD, Item::Type::JADE_SWORD}))
        {
            DAMAGE = -3;

            PreText += "[SWORDPLAY] ";
        }
        else if (Character::VERIFY_SKILL(player, Skill::Type::UNARMED_COMBAT))
        {
            PreText += "[UNARMED COMBAT] ";

            DAMAGE = -4;
        }
        else
        {
            PreText += "There is nothing you can do to prevent yourself being overwhelmed and slain.";

            Type = Story::Type::DOOM;
        }

        if (DAMAGE != 0)
        {
            PreText += "You LOSE " + std::to_string(-DAMAGE) + " Life Points.";
        }

        Character::GAIN_LIFE(player, DAMAGE);

        Text = PreText.c_str();
    }

    int Continue(Character::Base &player) { return 258; }
};

class Story329 : public Story::Base
{
public:
    Story329()
    {
        ID = 329;

        Text = "You were optimistic. It takes you nearly two hours to smash a hole large enough to squeeze through. As you stand on the ledge, panting with exhaustion and with sweat pouring off your body, there is a movement from inside the tomb.\n\nOut of the hole slithers a flying cobra. Its hood is vastly extended to form translucent oval wings which beat slowly, carrying the snake forward through the air with venatic precision. Its iridescent scales and hood make it seem like a polished stone effigy, but there is no mistaking the living menace in its glittering golden eyes and flickering forked tongue.\n\nAfter the hours spent pounding away at the slab, your shoulders are aching and you can barely lift your arms. There is no way you could climb back down to the canoe right now. The cobra swoops higher, arching its head as it hovers just above you. You must think of something quickly, or you will fall prey to its lethal bite.";

        Image = "images/filler1.png";

        Choices.clear();
        Choices.push_back(Choice::Base("[CHARMS] Cast a protective enchantment", 394, Skill::Type::CHARMS));
        Choices.push_back(Choice::Base(Skill::UNARMED_COMBAT.Name, 191, Skill::Type::UNARMED_COMBAT));
        Choices.push_back(Choice::Base("Use the MAN OF GOLD", 413, Item::Type::MAN_OF_GOLD));
        Choices.push_back(Choice::Base("Otherwise", 145));

        Controls = StandardControls();
    }
};

class Story330 : public Story::Base
{
public:
    Story330()
    {
        ID = 330;

        Text = "By the time you return with a little of the river water cupped in your hands, the strange dwarfish man has disappeared. Presumably he has crawled off into the reeds. You scan the bleak landscape, straining your ears for the sound of someone sloshing through the wet mud, but to no avail. He looked so weak that he could not get far, but you are not keen to go wading off the path in search of him. At least you know that you tried to help.";

        Image = "images/filler1.png";

        Choices.clear();

        Controls = StandardControls();
    }

    int Continue(Character::Base &player)
    {
        return 373;
    }
};

class Story331 : public Story::Base
{
public:
    Story331()
    {
        ID = 331;

        Text = "You can go to speak with the dead king's spirit tonight or you can rest here and decide what to do the next morning.";

        Image = "images/filler1.png";

        Choices.clear();
        Choices.push_back(Choice::Base("Go and talk to the Dead King's Spirit tonight", 218));
        Choices.push_back(Choice::Base("Go west towards Ashaka", 8, Choice::Type::LIFE, 1));
        Choices.push_back(Choice::Base("Go north to the coast", 30, Choice::Type::LIFE, 1));
        Choices.push_back(Choice::Base("Stay in Nachan for the festival", 416, Choice::Type::LIFE, 1));

        Controls = StandardControls();
    }
};

class Story332 : public Story::Base
{
public:
    Story332()
    {
        ID = 332;

        Text = "\"I notice you suffer from a bad back,\" you say to the fenman. \"May I ask why you don't rig a sail on your boat to save yourself the effort of rowing?\"\n\n\"Do you know nothing about boats?\" he grumbles as he picks up another heavy pot. \"A vessel made of reeds is to flimsy to support the weight of a mast.\"\n\n\"Use a double mast, so that the sail straddles the boat,\" you suggest. \"This would distribute the load more evenly. Also, why not treat the reeds with oil, which would make the hull more watertight?\"\n\n\"With the price of oil today?\" he cries incredulously. \"Reeds cost next to nothing.\"\n\n\"Ah, but think of the time it takes you to make a new boat for each river trip. A hull treated with oil would soon repay the initial cost.\"\n\nHe stops what he is doing and looks at you with new interest. \"Anything else?\" he asks.\n\n\"Yes. I see you are loading empty pots aboard your boat, presumably for sale downriver. Mead is plentiful in Nachan but not in the fens, so why not fill the pots with mead? It would take up no more space, but your profits would be enlarged by at least one fifth.\"\n\n\"A fine idea,\" he admits, \"but the pots are heavy enough for my poor back already.\"\n\n\"Exactly why you need a passenger who's willing to chip in a help,\" you answer in a trice. Convinced you are worth having along, he agrees to take you downriver for free..";

        Image = "images/filler1.png";

        Choices.clear();

        Controls = StandardControls();
    }

    int Continue(Character::Base &player) { return 355; };
};

class Story333 : public Story::Base
{
public:
    Story333()
    {
        ID = 333;

        Text = "Lifting the pitcher, you are horrified to see a second head on the woman's shoulder. It has a long lank hair, white skin and ghastly black lips. As you stifle a gasp of horror, its eyes snap open and it gives a chilling screech. The woman's arms lash out like a sleepwalker's dashing you backwards with astonishing strength.\n\nYou LOSE 1 Life Point.";

        Image = "images/filler1.png";

        Choices.clear();

        Controls = StandardControls();
    }

    void Event(Character::Base &player)
    {
        Character::GAIN_LIFE(player, -1);
    }

    int Continue(Character::Base &player) { return 104; };
};

class Story334 : public Story::Base
{
public:
    Story334()
    {
        ID = 334;

        Text = "The monster is impervious to your blows. More strands of long black hair wrap themselves around you. It is like fighting an octopus. You can do nothing to prevent it dragging itself closer until finally its jaws close on your windpipe. You feel yourself weakening, and then there is a crunching of cartilage and a pink spray of blood. Your blood.";

        Image = "images/flying-head.png";

        Type = Story::Type::DOOM;

        Choices.clear();

        Controls = StandardControls();
    }
};

class Story335 : public Story::Base
{
public:
    Story335()
    {
        ID = 335;

        Text = "\"You seek to enter the underworld,\" says the spirit. \"The way will not be easy. But I perceive your cause to be honourable, so I shall aid you.\"\n\nYou do not dare to answer. He goes on: \"You must place a JADE BEAD under your tongue and keep it there until you reach the four paths, not speaking whatever the temptation. Once you are at the four paths, you may remove the bead from your mouth but you should retain it until you see a kapok tree. Before any of this you must cross Death Canyon, however. If you have no other option, seek the dragon Kawak who lies across the canyon with one of his two heads on either side. Others have passed through his jaws and emerged at the far end, and if you are dauntless you may be able to do this too.\"\n\nThere is deep silence after he finishes speaking. You wait with lowered gaze. It is not seemly to look upon even a living king unless invited to. Suddenly there is a flash and an immediate peal of thunder, followed by a deluge of heavy tropical rain.\n\nYou look up. King Sky Shield and the phantom snake have vanished, but the shrine is not as it was before. Now a pit has opened in the floor, revealing stairs down into the pyramid. It is your route to the underworld.";

        Image = "images/filler1.png";

        Choices.clear();
        Choices.push_back(Choice::Base("Venture down the steps", 9));
        Choices.push_back(Choice::Base("Leave the shrine and wait until morning to resume your journey overland to Ashaka", 8));
        Choices.push_back(Choice::Base("Go downriver to the coast", 30));

        Controls = StandardControls();
    }
};

class Story343 : public Story::Base
{
public:
    Story343()
    {
        ID = 343;

        Text = "You sail on until the sight of circling gulls tells you that land is near. You know that it cannot be mainland. As a green swathe of foliage appears in the distance you realize you have found the fabled Isle of the Iguana.\n\nGetting closer, you find a coast of high cliffs which the sea has pounded into fanciful shapes. You sail under an arch of white rock and around a headland that reminds you of a serpent's mouth, arriving at last at a stretch of pebble-strewn beach.\n\n\"We must be virtually due east of Tahil,\" you announce. \"Certainly by sailing with the rising sun at our back we'd soon reach the mainland.\" But even as you are saying this, your eyes are scanning the shore with avid curiosity.";

        Image = "images/filler1.png";

        Choices.clear();
        Choices.push_back(Choice::Base("Put in at the island", 366));
        Choices.push_back(Choice::Base("Sail on to Tahil", 300));

        Controls = StandardControls();
    }
};

class Story344 : public Story::Base
{
public:
    Story344()
    {
        ID = 344;

        Text = "Cool shade embraces you as you step into the shop's interior. Your eyes adjust to the darkness. You call for service. A man rouses himself from his siesta and comes forward, blinking sleepily at you where you stand in the sunlit doorway as though you were a spirit that had emerged from the afterlife.\n\n\"I am journeying into the desert,\" you announce, to his obvious amazement. \"I'll need supplies.\"\n\nHe shows you the few items he has for sale in this impoverished town.";

        Image = "images/filler1.png";

        Choices.clear();

        Shop = {
            {Item::Type::WATERSKIN, 4},
            {Item::Type::MAGIC_WAND, 16},
            {Item::Type::FLINT_KNIFE, 1},
            {Item::Type::BLOWGUN, 3},
            {Item::Type::BLANKET, 2}};

        Controls = ShopControls();
    }

    int Continue(Character::Base &player) { return 325; }
};

class Story345 : public Story::Base
{
public:
    Story345()
    {
        ID = 345;

        Text = "The invisible demons of the desert are known to harness whirlwinds, which they ride pell-mell across the sand. It is said that on nights of the full moon they can even be seen crouched atop their captive whirlwinds -- translucent unhuman figures with their faces raised shrieking to the sky.\n\nThe moon is not full tonight, and you are glad. You have no wish to see these monsters. Your only desire is to get rid of them, and to do that you must exploit their one weakness: once in full pursuit of a victim, they cannot veer quickly off a straight line.";

        Image = "images/filler1.png";

        Choices.clear();

        Controls = StandardControls();
    }

    int Continue(Character::Base &player) { return 137; }
};

class Story346 : public Story::Base
{
public:
    Story346()
    {
        ID = 346;

        Text = "The high priest looks disappointed, but seems to understand when you explain that you have barely enough to purchase the supplies you will need for the journey.\n\n\"A coil of ROPE might not be a bad idea,\" he suggests. \"I've often found the ROPE to come in handy. Oh, and a decent SWORD of course -- assuming you know how to use one.\" He juggles the heavy rubber ball he's carrying from one hand to the other. \"What about a practice session in the arena before you head off?\"";

        Image = "images/filler1.png";

        Choices.clear();
        Choices.push_back(Choice::Base("Practice the ball contest with him", 368));
        Choices.push_back(Choice::Base("Get on with preparations for your journey", 93));

        Controls = StandardControls();
    }
};

class Story347 : public Story::Base
{
public:
    Story347()
    {
        ID = 347;

        Text = "No sooner have you released your tight grip on the SHAWL than it is snatched away. There is a whispered cry of exultation from all around. You have your only clear view of the stabaie as they shoot off into the distance like flickers of silent lightning, to be swallowed up by the gloom of the forest within moments. Silence fills the glade, and you realize you are alone in the heart of the wood.";

        Image = "images/filler1.png";

        Choices.clear();

        Controls = StandardControls();
    }

    void Event(Character::Base &player)
    {
        Character::LOSE_ITEMS(player, {Item::Type::SHAWL});
    }

    int Continue(Character::Base &player) { return 324; }
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
        Choices.push_back(Choice::Base("Pay a visit to the market", 142));
        Choices.push_back(Choice::Base("Head north out of the city", 120));
        Choices.push_back(Choice::Base("Take the southern road towards the forest", 165));

        Controls = StandardControls();
    }
};

class Story354 : public Story::Base
{
public:
    Story354()
    {
        ID = 354;

        Text = "The wall around the royal precinct is constructed of closely fitted blocks of smooth limestone. The builders even bowed it out slightly to make it more difficult to climb. You loiter casually at a corner until there is no one in sight, then brace yourself around the bend in the wall and pull yourself up.\n\nThe top of the wall tapers to a wedge which is rimmed with spikes of sharp obsidian, provoking a grin of admiration from you for the ingenuity of the royal architects. But none of these measures is enough to deter you. Swinging into a cartwheel, you vault over the top of the wall without touching the razor-edge of obsidian, dropping lightly to your feet on the other side.\n\nA glance towards the gate confirms that the guards heard nothing: they are still gazing stolidly into the street outside. You slink away in the twilight and climb the steps of the pyramid.";

        Image = "images/filler1.png";

        Choices.clear();

        Controls = StandardControls();
    }

    int Continue(Character::Base &player) { return 415; }
};

class Story355 : public Story::Base
{
public:
    Story355()
    {
        ID = 355;

        Text = "You spend a couple of pleasant days on the river, with nothing to do but watch the green banks slide past while you trail your fingers in the water and swap stories with the fenman. Like most people who travel widely, he has a fund of folktales with which to regale you.\n\n\"I have enjoyed having you along,\" he says as he ties up his boat at a village, \"but now we have arrived at my home. The coast is only a few days' walk -- just follow the road which leads along the riverbank, and may the God of the Pole Star guide you safely on your quest.\"\n\n\"Thank you,\" you reply, \"and may the God of the River see that you find good fortune in your business.\"\n\nYou have gone only a short distance when he calls out after you. \"Incidentally I should warn you that the fens are infested with nightcrawlers. I take it you know how to deal with such creatures?\"";

        Image = "images/filler1.png";

        Choices.clear();
        Choices.push_back(Choice::Base("[FOLKLORE] Assure him that you are wise to such magical menaces", 264, Skill::Type::FOLKLORE));
        Choices.push_back(Choice::Base("Perhaps you had better ask him to explain further", 172));

        Controls = StandardControls();
    }
};

class Story356 : public Story::Base
{
public:
    Story356()
    {
        ID = 356;

        Text = "You walk on until overtaken by nightfall and exhaustion. Sitting down under the spreading foliage of a calabash tree, you consider looking for a more comfortable place to spend the night. You have passed a few peasant huts among the canals lining your route, but at the moment you cannot see any glimmers of lamplight to indicate habitation. Since it is a warm and sultry night, you decide to sleep out in the open.\n\nYou are woken by something heavy falling on your shoulder. It is pitch dark. Your heart is thudding, your nerves taught with unreasoning fear. You hear a slurping sound and turn your head. A horrible inhuman face is pressed up close to yours. Its lips are slick with your blood, but that is not the worst of it. You see that the monstrous head is sprouting from your own shoulder, and then you start to scream.\n\nYou have come to a grisly end.";

        Image = "images/filler1.png";

        Type = Story::Type::DOOM;

        Choices.clear();

        Controls = StandardControls();
    }
};

class Story357 : public Story::Base
{
public:
    Story357()
    {
        ID = 357;

        Text = "The head is unable to take control of you. Repulsed, it falls back with a snarl of icy rage. The enmeshing strands of hair go slack, but at best you have only a moment's respite before it resumes its attack.";

        Image = "images/flying-head.png";

        Choices.clear();
        Choices.push_back(Choice::Base("Strike it with your fist", 334));
        Choices.push_back(Choice::Base("Dash it against the trunk of the nearest tree", 378));

        Controls = StandardControls();
    }
};

class Story366 : public Story::Base
{
public:
    Story366()
    {
        ID = 366;

        Text = "Cowering under the trees at the back of the beach you find a very old man. His clothing is ragged, but the few scraps of adornment that he retains indicate a person of wealth and prestige. This impression is confirmed when he opens his mouth, saying, \"If you have come to kill me, I pray that you do so quickly, at least.\"\n\n\"We haven't come to kill you,\" you reply, extending your hand in friendship.\n\nHe returns a hopeful smile. \"I am Jade Thunder, once a great wizard. A contest with my arch-rival brought me to this desolate spot, and here we fought our last battle. I slew him, but with his dying breath he sealed my wand within a barrier of fire and now I cannot use my magic to return home.\"\n\nYou go with him to a spot further up the beach. Great magic has obviously been at work here, impossibly warping the trees and leaving the coconuts with silent staring faces. The sand underfoot has a dozen colours. In the centre of the clearing, a circle of crackling green flame surrounds a wand.\n\nYou try helping Jade recover his wand.";

        Image = "images/filler1.png";

        Choices.clear();
        Choices.push_back(Choice::Base(Skill::CUNNING.Name, 21, Skill::Type::CUNNING));
        Choices.push_back(Choice::Base("[SPELLS] Cast spells with your own wand", 435, Choice::Type::SKILL_ANY, Skill::Type::SPELLS, {Item::Type::MAGIC_WAND, Item::Type::JADE_SWORD}));
        Choices.push_back(Choice::Base(Item::Descriptions[Item::Type::MAN_OF_GOLD], 45, Item::Type::MAN_OF_GOLD));
        Choices.push_back(Choice::Base("Walk boldly into the flames", 68));
        Choices.push_back(Choice::Base("Decide against helping him", 437));

        Controls = StandardControls();
    }
};

class Story367 : public Story::Base
{
public:
    Story367()
    {
        ID = 367;

        Text = "\"In times gone by, heretics and madmen were cast out from the town by this gate,\" says one of the guards. \"As they went, some would scratch pictures of what they expected to find in the far west.\"\n\n\"That's why it's called the Gate of Exiles,\" says the other man. \"I reckon you must be one of the few people who've taken this route by choice.\"\n\n\"I didn't have a choice,\" you say.\n\nTaking up your pack, you walk out into the waiting desert.";

        Image = "images/filler1.png";

        Choices.clear();

        Controls = StandardControls();
    }

    int Continue(Character::Base &player) { return 407; };
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

    void Event(Character::Base &player)
    {
        Character::GET_CODEWORDS(player, {Codeword::Type::POKTAPOK});
    }

    int Continue(Character::Base &player) { return 389; }
};

class Story369 : public Story::Base
{
public:
    Story369()
    {
        ID = 369;

        Text = "You make a great show of folding the SHAWL and tucking it tightly under your arm. \"You've lured me far off the beaten track and caused me to inhale an insect!\" you say aloud, feeling somewhat foolish since you cannot see the creatures you are addressing.\n\n\"Merely a prank,\" says the eerie voice at your shoulder. \"Give us back our SHAWL, and we'll lead you back to the path without delay.\"\n\n\"Better yet,\" offers another of the stabai, \"we know of a great treasure hidden near by. If we take you to it, will you return the SHAWL?\"\n\nYou know better than to hand the SHAWL back before they have completed their part of any bargain. Even then, it might be wise to retain it in case they try to cause you further trouble. You decide to make no firm promises for now.";

        Image = "images/filler1.png";

        Choices.clear();
        Choices.push_back(Choice::Base("Ask them to show you the treasure", 409));
        Choices.push_back(Choice::Base("Insist on being led back to the trail you were following before", 390));

        Controls = StandardControls();
    }
};

class Story370 : public Story::Base
{
public:
    Story370()
    {
        ID = 370;

        Text = "Midnight Bloom proves to be a briskly efficient young woman with a vivacious smile. A jade necklace inlaid with flecks of gold sets off her deep coppery tan to good effect. She reads the letter given to you by the Matriarch and nods, saying, \"Many traders are reluctant to go as far as Tahil now, because the collapse of the Great City has left bands of brigands roaming unchecked in the region. But luckily I shall be sailing there in a week's time, to tie up some loose ends in the clan's business.\"\n\nYou spend a restful week in Balak at the house of your relatives here. Your Life Points are fully restored. At last Midnight Bloom comes to tell you that preparations are complete. Tomorrow you set sail for Tahil.\n\nYou gained the codeword SAKBE.";

        Image = "images/filler1.png";

        Choices.clear();

        Controls = StandardControls();
    }

    int Continue(Character::Base &player) { return 182; }

    void Event(Character::Base &player)
    {
        player.Life = player.MAX_LIFE_LIMIT;

        Character::GET_CODEWORDS(player, {Codeword::Type::SAKBE});
    }
};

class Story371 : public Story::Base
{
public:
    Story371()
    {
        ID = 371;

        Text = "There is a gasp form the crowd as they watch you jump over the edge. You know you must act quickly. Pulling off the helmet as you tumble down, you hug it against you an instant before the lake surface comes rushing up, driving the air out of your lungs with its chilling embrace. You are surrounded by silence and watery gloom. The weight of gold drags you down, but you have managed to trap a pocket of air inside the helmet and you use this to breathe while unfastening the straps of the regalia. All the air is used up by the time you struggle free, and a red haze swims in front of your vision as you kick up towards the surface.\n\nYou reach fresh air to discover you are no longer at the bottom of the well. Instead of open sky, a cavern roof stretches overhead. A ghostly grey light emanates out of nowhere. Trudging up onto a a rocky ledge, you hear the sound of oars. A canoe is approaching. As it takes shape out of the shadows, you get a good look at the two occupants. They are like nothing you have ever seen outside your night-mares. Now you know you truly have plunged into the fabled river that leads between the world of the living and the world of the dead.";

        Image = "images/filler1.png";

        Choices.clear();

        Controls = StandardControls();
    }

    int Continue(Character::Base &player) { return 97; }
};

class Story374 : public Story::Base
{
public:
    Story374()
    {
        ID = 374;

        Text = "Not far away, down a backstreet close to the palace wall, you find a house with a plump young pig tethered outside. Listening at the window, you hear the occupants of the house rowdily enjoying a few cups of mead. You tiptoe back to the pig and, stifling its squeals, tuck it under your arms before returning to the palace gate. There, crouched out of sight around the corner of a building, you release the pig and give it a shove which carries it towards the guards.\n\nThe pig takes a few steps, gives a baffled grunt upon confronting the guards, then bolts past them. \"Grab it, lads!\" cries the guard in high spirits. \"What fine succulent feast it'll make tomorrow, eh?\"\n\nTwo or three of the guards go chasing off after the pig, and the others look on laughing at their comrades' antics. You sneak past while their backs are turned and make your way over to the pyramid of the dead king.";

        Image = "images/filler1.png";

        Choices.clear();

        Controls = StandardControls();
    }

    int Continue(Character::Base &player) { return 415; }
};

class Story378 : public Story::Base
{
public:
    Story378()
    {
        ID = 378;

        Text = "Grabbing a long hunk of hair, you bunch your muscles and swing the gibbering head around in a swift arc. It strikes the tree with a noise like a melon splitting, and a loathsome torrent of ichor spills out of its broken skull as it drops heavily to the ground. You watch in disgust as its eyes roll up and it gives a last fetid gasp. The twitching tresses shrivel before your eyes, and the head withers rapidly to a desiccated lifeless shell.";

        Image = "images/flying-head.png";

        Choices.clear();

        Controls = StandardControls();
    }

    void Event(Character::Base &player)
    {
        Character::REMOVE_CODEWORD(player, Codeword::Type::SALVATION);
    }

    int Continue(Character::Base &player) { return 398; }
};

class Story387 : public Story::Base
{
public:
    Story387()
    {
        ID = 387;

        Image = "images/filler1.png";

        Controls = StandardControls();
    }

    void Event(Character::Base &player)
    {
        Choices.clear();

        if (Character::VERIFY_SKILL(player, Skill::Type::SEAFARING))
        {
            Text = "You repair the boat and set sail.";

            Character::REMOVE_CODEWORD(player, Codeword::Type::EB);
        }
        else
        {
            Text = "You are stranded unless you risk helping Jade Thunder.";

            Choices.push_back(Choice::Base("[SPELLS] Use a wand", 435, Choice::Type::SKILL_ANY, Skill::Type::SPELLS, {Item::Type::MAGIC_WAND, Item::Type::JADE_SWORD}));
            Choices.push_back(Choice::Base(Skill::CUNNING.Name, 21, Skill::Type::CUNNING));
            Choices.push_back(Choice::Base(Item::Descriptions[Item::Type::MAN_OF_GOLD], 45, Item::Type::MAN_OF_GOLD));
            Choices.push_back(Choice::Base("Otherwise", 68));
        }
    }

    int Continue(Character::Base &player) { return 300; }
};

class Story388 : public Story::Base
{
public:
    Story388()
    {
        ID = 388;

        Text = "\"It is protection against the dust demons,\" explains one of the guards. \"They are invisible sprites who whip up whirlwinds and drive them like ravaging beasts onto those who travel in the desert.\"\n\nYou cast a brief glance at the boulder. \"Not much protection, surely? The demons could just steer their whirlwinds around it.\"\n\n\"No,\" says the other man in a tone that suggests they tell this legend to every wayfarer, \"because dust demons can only travel in long straight paths. They can't turn easily.\"\n\n\"Or so the priest Spitting Viper told us,\" chimes in the first man.\n\nBidding them farewell, you take up your pack and walk out into the waiting desert.";

        Image = "images/filler1.png";

        Choices.clear();

        Controls = StandardControls();
    }

    int Continue(Character::Base &player) { return 407; };
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

    void Event(Character::Base &player)
    {
        Character::GET_ITEMS(player, {Item::Type::JADE_BEAD, Item::Type::MAIZE_CAKES});
    }

    int Continue(Character::Base &player) { return 25; }
};

class Story390 : public Story::Base
{
public:
    Story390()
    {
        ID = 390;

        Text = "The stabai flicker at the edges of your vision, thin elusive shards of paleness against the green-tinged shadows. They lead you on through silent glades surrounded by trees which stretch endlessly upwards as though supporting the sky. The moist heat of the jungle weighs upon you until you feel like a drowning man. Slogging on through the eerie silence, limbs aching with fatigue, you begin to suspect the stabai are not leading you to safety at all. What if their plan is to entice you deeper and deeper into the woods until you finally drop from hunger and exhaustion?";

        Image = "images/filler1.png";

        Choices.clear();
        Choices.push_back(Choice::Base("Dismiss the stabai and try to find your own way back to the trail", 324));
        Choices.push_back(Choice::Base("Return the SHAWL in the hope of gaining the stabai's goodwill", 347, Item::Type::SHAWL));
        Choices.push_back(Choice::Base("Continue to let them guide you", 168));

        Controls = StandardControls();
    }
};

class Story391 : public Story::Base
{
public:
    Story391()
    {
        ID = 391;

        Text = "You strike up a conversation with a trader whose boat looks impressively large and seaworthy. He is putting together a cargo of richly dyed cotton and woven feather head-dresses which he hopes to sell in Tahil.\n\n\"Brigands and pirates abound since the fall of the Great City,\" he says. \"These are exciting times we live in. Many prefer to cower in safety close to home, but for a bold man there are fine profits to be made.\"\n\nA few judicious remarks about the weather and the tides convince him that you would be a worthwhile person to have along on the voyage. He tells you to return in a week, when it will be time to set out.";

        Image = "images/filler1.png";

        Choices.clear();

        Controls = StandardControls();
    }

    int Continue(Character::Base &player) { return 205; }
};

class Story392 : public Story::Base
{
public:
    Story392()
    {
        ID = 392;

        Text = "You know that your first priority must be to get out of the encumbering golden regalia as soon as you hit the water, otherwise it will drag you down to your doom. Your fingers have already found the straps as you dive off the platform. You hear a moan of awe from the onlookers which is soon snatched away by the rush of wind as you fall. Then you hit the water, and suddenly you are sinking through silent darkness.\n\nAnother person might panic, losing precious seconds, but you have been in this kind of scrape before. You made sure to take a good breath of air before going under, and you make use of that now to sustain you while you struggle free of the regalia. Discarding it, you watch it go drifting down until the yellow glints of metal are lost in the murk. It is a priceless treasure, but the loss of it gladdens your heart as you strike up through the cold green water.\n\nYour head breaks the surface and you suck gratefully at the stale air of an underground cavern. There is no flicker of daylight. The sinkhole has vanished, and the only light is now an eldritch grey glow seeping from an unknown source. You guess that you have been carried to the outer fringes of the underworld. Stumbling up onto a pebble-strewn shore, you gaze out into the gloom and see a canoe coming towards you. Your guess is confirmed when you see the oarsmen. They are like no creatures of the mortal realm.";

        Image = "images/filler1.png";

        Choices.clear();

        Controls = StandardControls();
    }

    int Continue(Character::Base &player) { return 97; }
};

class Story396 : public Story::Base
{
public:
    Story396()
    {
        ID = 396;

        Text = "You crouch behind some sacks of grain and wait an hour or so until the streets are completely deserted. A few lights show from the palace windows. Beside the gate, a brazier flares in the night breeze. Other than that it is completely dark. When the moon shows as a misty patch of silver beyond the wooded hills to the east, you put your plan into action.\n\n\"Brr, it's cold suddenly,\" remarks one of the guards at the gate.\n\nOne of his companions nods. \"Did you hear something just then?\" He casts a nervous glance over his shoulder. The pyramid-tomb of the dead king looms darkly against the pale halo of moonlight.\n\n\"Now you come to mention it...\" says another, also looking back. Suddenly he gasps, eyes popping out of his head. \"By all the gods!\"\n\nA parade of ghosts descends the steps of the pyramid. Their dead faces are formed on mist and moonbeams, and they move without sound across the courtyard. The guards are brave enough to face any foe of flesh and blood, but the sight of this host of spectres sends icy terror through their veins. Dropping their weapons, they run off into the night.\n\nYou watch them go, then step out from your hiding-place. As you cancel your illusion spell, the 'ghosts' dissolve into the darkness. You hurry across to the pyramid before the guards recover enough nerve to come slinking back.";

        Image = "images/filler1.png";

        Choices.clear();

        Controls = StandardControls();
    }

    int Continue(Character::Base &player) { return 415; }
};

class Story398 : public Story::Base
{
public:
    Story398()
    {
        ID = 398;

        Text = "The woman is in a deep trance, but you shake her until she comes out of it. She tells you that she is a sorceress who fell victim to the nightcrawler when she tried to exorcize it from the region. \"I can never repay my debt to you,\" she says, weeping with relief to be free of the noxious creature's control at last. \"When you get to the sea, follow the shore until you come to a giant stone idol which is buried up to its neck in the sand. This is the idol of an ancient god. Tell him the number of stars in the sky and he will speak to you.\"\n\n\"How many stars are there?\"\n\nShe smiles. \"Tell him a hundred thousand million and seven. He wouldn't believe the true answer.\"\n\nYou gained the codeword OLMEK.\n\nBidding the woman farewell, you continue on your way.";

        Image = "images/filler1.png";

        Choices.clear();

        Controls = StandardControls();
    }

    void Event(Character::Base &player)
    {
        Character::GET_CODEWORDS(player, {Codeword::Type::OLMEK});
    }

    int Continue(Character::Base &player) { return 417; }
};

class Story400 : public Story::Base
{
public:
    std::string PreText = "";

    Story400()
    {
        ID = 400;

        Image = "images/filler1.png";

        Choices.clear();

        Controls = StandardControls();
    }

    void Event(Character::Base &player)
    {
        PreText = "You lose your grip on the crumbling bark and go crashing down through the branches.";

        auto DAMAGE = -5;

        PreText += "\n\n";

        if (Character::VERIFY_SKILL(player, Skill::Type::AGILITY))
        {
            PreText += "[AGILITY] You manage to break your fall by catching a stout strangler root.";

            DAMAGE = -1;
        }
        else
        {
            PreText += "The ground comes up to hit you with a crushing force.";
        }

        PreText += "\n\nYou LOSE " + std::to_string(-DAMAGE) + " Life Points.";

        Character::GAIN_LIFE(player, DAMAGE);

        if (player.Life > 0)
        {
            PreText += "\n\nStruggling to your feet, you limp away before the creature can descend from its arboreal lair to pursue you. The stabai have flitted away, terrified of the creature's wrath. When you are safely away from the tree, you clean the muck off the diadem and hold it up in a shaft of sunlight. It is a gold circlet of the sort that sometimes adorn the heads of great nobles. Sacred symbols are stamped into the metal and there is a jade inlay in the fourfold shape of the World Tree, the source of life itself in the mythology of your people (GOLD DIADEM).";

            Character::GET_ITEMS(player, {Item::Type::GOLD_DIADEM});
        }

        Text = PreText.c_str();
    }

    int Continue(Character::Base &player) { return 324; }
};

class Story406 : public Story::Base
{
public:
    Story406()
    {
        ID = 406;

        Text = "The storm blows itself out towards dawn, leaving you all shaking with exhaustion. You look around. Unbroken sea surrounds you. The only clue to your course is the sun boiling its way through a bank of low cloud across the horizon, but the sight of circling gulls tells you that land is near. You know that it cannot be the mainland. As a green swathe of foliage appears in the distance, you realize you have found the fabled Isle of the Iguana.\n\nGetting closer, you find a coast of high cliffs which the sea has pounded into fanciful shapes. You sail under an arch of white rock and around a headland that reminds you of a serpent's mouth, arriving at last at a stretch of pebble-strewn beach.\n\n\"The ship was badly damaged in the storm,\" says one of the sailors, pointing to the water collecting in the bottom of the hull. \"We must put in here for repairs.\"\n\nYou gained the codeword EB.";

        Image = "images/filler1.png";

        Choices.clear();

        Controls = StandardControls();
    }

    void Event(Character::Base &player)
    {
        Character::GET_CODEWORDS(player, {Codeword::Type::EB});
    }

    int Continue(Character::Base &player) { return 366; }
};

class Story407 : public Story::Base
{
public:
    Story407()
    {
        ID = 407;

        Text = "The sun looks like a funnel of flame in the shimmering oven of the sky. Through the soles of your sandals, the rock and sand feel as hot as cinders. The air seems thick with dust, but at nightfall the temperature plummets and you are chilled by a strong breeze.";

        Image = "images/filler1.png";

        Controls = StandardControls();
    }

    void Event(Character::Base &player)
    {
        Choices.clear();

        if (!Character::VERIFY_SKILL(player, Skill::Type::WILDERNESS_LORE))
        {
            Choices.push_back(Choice::Base("Travel by day", 22));
            Choices.push_back(Choice::Base("Travel by night", 46));
        }
    }

    int Continue(Character::Base &player)
    {
        return 434;
    }
};

class Story408 : public Story::Base
{
public:
    Story408()
    {
        ID = 408;

        Text = "The high priest winds a white cloth across your eyes and leads you through to the inner shrine. A deep chill abides here; the thick stone blocks of the Death God's temple walls are never warmed by the sun. The sweet tarry smell of incense hangs in the air. You feel a hand on your shoulder, guiding you to kneel.\n\nA long period of utter silence ensues. You did not hear the high priest withdraw from the chamber, but you gradually become sure that he has left you here alone. You dare not remove the blindfold; to gaze directly on the holy of holies would drive you instantly insane.\n\nA whispering slithers slowly out of the silence. At first you take it for a trick of your unsettle imagination, but by straining your ears you begin to make out words. \"The way to the west lies through the underworld,\" the whispering tells you. \"Go to the city of Yashuna. North of the city lies a sacred well which is the entrance to the underworld. Take this path, which is dangerous but swift, and you will emerge at the western rim of the world. From there it is but a short journey back through the desert to your goal.\"\n\nThe whispers fade, drowned out by the thudding of your heart. Frozen with terror at the words of the god, you crouch motionless on the cold flagstones. The cloying scent of incense grows almost unbearable.\n\nSuddenly a hand touches your shoulder. After the initial jolt of alarm, you allow yourself to be led out onto the portico of the temple, where the blindfold is removed. You blink in the dazzling sunlight. You feel as weak as a baby and the smell of incense clings to your clothes. After the cool of the shrine, the heat of the afternoon sun makes you feel slightly sick.\n\nThe podgy priest is looking up into your eyes. \"You head the voice of the god,\" he says simply.\n\nYou gained the codeword CENOTE.";

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

class Story409 : public Story::Base
{
public:
    Story409()
    {
        ID = 409;

        Text = "The stabai guide you through the forest to a tall tree which has had the vitality leeched out of it by a strangler fig. The roots of the strangler enclose the decomposing trunk like a crusty scab. A barely glimpsed arm directs your attention to a hole some way up the side of the tree. \"There,\" announces the fluting voice of the stabai. \"The treasure is inside the dying tree-trunk.\"\n\nThe strangler's roots make it an easy climb. The stabai effortlessly keep pace with you. From somewhere amid the branches you hear them urging you to return the SHAWL. \"Not yet,\" you reply. \"First I'll take a look at this treasure.\"\n\n\"But then you will return the SHAWL?\" they whine.\n\nYour foot slips but you steady yourself in the crook of a branch. It is a long way to the ground. No doubt the stabai are hoping you will break your neck, and their distractions are not helping. \"I'm making no promises,\" you tell them irritably.\n\nPulling yourself up level with the hold, you peer inside. It looks dank and rotten. There is a smell like mushrooms -- sickly sweet, a heady aroma. But as your eyes penetrate the darkness, you see that the stabai did not lie. Just within reach glitters a solid GOLD DIADEM.";

        Image = "images/filler1.png";

        Controls = StandardControls();
    }

    void Event(Character::Base &player)
    {
        Choices.clear();

        if (!Character::VERIFY_SKILL(player, Skill::Type::ROGUERY))
        {
            Choices.push_back(Choice::Base("Reach in and take the diadem", 49));
            Choices.push_back(Choice::Base("Climb back down and get the stabai to lead you out of the woods", 390));
        }
    }

    int Continue(Character::Base &player) { return 26; }
};

class Story411 : public Story::Base
{
public:
    Story411()
    {
        ID = 411;

        Text = "The high priest watches the sun decline across the treetops to the west. \"The moment has arrived,\" he intones to the waiting crowd. \"This ambassador will bear our message to the Rain God.\"\n\nYou stare down. A gulf of dozen man-heights separates you from the grim watery gate of the afterlife. Even with your spell in force, you cannot suppress a qualm as you step out into space.\n\nThe gasp of the crowd is lost in the rush of wind as you go plummeting down. The white root-tangled walls of the sinkhole flicker past your vision. An instant later there is an icy impact and then darkness and eeriness surround you.\n\nIf not for your magic, the golden regalia you are wearing would bear you down to a watery grave. You struggle free of the fastenings and allow the vest of gold plaques to sink out of sight in the murk, but you keep the GOLDEN HELMET tucked under your arm as you go shooting up towards the surface.\n\nYou break the surface like a log bobbing up out of the depths. There is a weak glow here, but it is not daylight. You are no longer at the bottom of the sinkhole, but in a vast underground cavern. You stagger up onto a rocky shore before your spell can wear off.\n\nThe sound of oars reaches your ears. Approaching you across the lake comes a long canoe crewed by two weird figures that seem to have strayed out of a feverish dream.";

        Image = "images/filler1.png";

        Choices.clear();
        Choices.push_back(Choice::Base("Keep the GOLDEN HELMET", 97, Choice::Type::GET_ITEM, Item::Type::GOLDEN_HELMET));
        Choices.push_back(Choice::Base("Leave it", 97));

        Controls = StandardControls();
    }
};

class Story415 : public Story::Base
{
public:
    Story415()
    {
        ID = 415;

        Text = "The shrine at the top of the pyramid is a small dark chamber fronted by a portico supported by strong square pillars. You can see the mouth of the spirit tube, fashioned in the shape of a slim snake. You know that the hollow tube extends all the way down through the interior of the pyramid to the tomb chamber deep in its heart. It is an eerie feeling to think that when you speak into the tube your words echo down into the ear of the dead king.\n\nYou keep a lone vigil long into the night. As the moon reaches its zenith, there is a soft soughing of wind up the spirit tube. Suddenly you realize you are unable to move. You will not admit to fear, but you are struck with awe to see a long curl of glowing green mist ooze out of the mouth of the tube. It hangs there in the shadows of the shrine, slowly curling in the air, gradually coming into focus in the form of a translucent snake. As you watch, the snake opens its jaws and the head of a man pushes into view, extruding from inside the snake's body until he has entirely emerged except for his left leg, which remains within its throat. He is a glistening red colour, as though covered in fresh blood, and as nude as a newborn baby.\n\nYou bow your head humbly to the cold flagstones, knowing that this is the spirit of King Sky Shield.";

        Image = "images/filler1.png";

        Choices.clear();

        Controls = StandardControls();
    }

    int Continue(Character::Base &player) { return 335; }
};

class Story416 : public Story::Base
{
public:
    Story416()
    {
        ID = 416;

        Text = "The festivities include ritual dance-dramas in which the participants mask themselves as beasts, gods and heroes from folklore. Accompanied by flutes and the throbbing beat of immense wooden drums, they swirl across the plaza in front of the temples, enacting the deeds of such mythic figures as the hero-twins Forethought and Afterthought.\n\nAt one point the twins take part in a ball contest and Afterthought succeeds in hurling the ball through the stone hoop on the side of the court. This provokes shouts of praise from spectators at the back of the crowd, but you are close to the front and can see that the apparently difficult shot was made with the help of string tied to the ball.\n\n\"Huh!\" snorts a woman standing next to you. \"He'd never have made the shot otherwise. I've never seen it done for real in all the contests I've watched.\"\n\nThe woman's son tugs at her skirt. He wants to know why the act of scoring in the ball contest has given him victory over his foes, the Lords of Death. \"That's because it's more than a game,\" you hear her saying. \"It's also a sacred ritual. You'll understand when you're older.\"\n\nThe afternoon wears on with much feasting and jollity. By evening you are happy to lie down in the plaza and drift off to sleep as the sultry night unfolds its canopy of stars. Tomorrow you must decide.";

        Image = "images/filler1.png";

        Choices.clear();
        Choices.push_back(Choice::Base("Continue overland to Ashaka", 8));
        Choices.push_back(Choice::Base("Go downriver to the coast", 30));

        Controls = StandardControls();
    }
};

class Story417 : public Story::Base
{
public:
    Story417()
    {
        ID = 417;

        Text = "You leave the fenlands and continue north. Arriving at last at the coast, you go to the edge of the sea and watch the setting sun gouge streaks of blood across a jade-grey sky.";

        Image = "images/filler1.png";

        Choices.clear();

        Controls = StandardControls();
    }

    void Event(Character::Base &player)
    {
        Choices.clear();

        if (!Character::VERIFY_CODEWORD(player, Codeword::Type::OLMEK))
        {
            Choices.push_back(Choice::Base("Set off walking along the coast towards Tahil", 228));
            Choices.push_back(Choice::Base("Try to get a passage on a ship", 251));
        }
    }

    int Continue(Character::Base &player) { return 260; }
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

class Story425 : public Story::Base
{
public:
    Story425()
    {
        ID = 425;

        Text = "You hold out the SHAWL, allowing the scaly talons to catch hold of it. The arm withdraws into the hole and there is a pause while the unseen creature examines what it has got. \"This is a stabai SHAWL!\" cries a voice full of insensate rage. \"A curse be upon you, you crafty stabai! May fungus infect your limbs and sickness burrow into your bodies. You'll not escape lightly for this outrage.\"\n\nThe stabai utter moans of dismay and melt away into the distance, alarmed by the monster's threats. For your part, you descend swiftly and quietly to examine your prize. Cleaning away the coating of muck, you discover it to be a circlet such as a nobleman or high priest might wear upon his brow. It is patterned with sacred glyphs and bears the cruciform symbol of the World Tree in inlaid plaques of jade. It is literally worth a king's ransom (GOLD DIADEM).";

        Image = "images/filler1.png";

        Choices.clear();

        Controls = StandardControls();
    }

    void Event(Character::Base &player)
    {
        Character::LOSE_ITEMS(player, {Item::Type::SHAWL});
        Character::GET_ITEMS(player, {Item::Type::GOLD_DIADEM});
    }

    int Continue(Character::Base &player) { return 324; };
};

class Story426 : public Story::Base
{
public:
    Story426()
    {
        ID = 426;

        Text = "Not far off, a young priestess is overseeing the work of a group of artisans. Out of curiosity you stroll over and see that they are decorating a temple wall with an overlay of stucco. Apprentices trowel the plaster into place and then senior artisans work with the speed and assurance of past masters, sculpting images of heroes, princes and gods as the priestess directs. Then, while the stucco is still damp, a second team steps in with pots of dye and applies bright colours. You can only marvel at the skill of the men's work.\n\nThe figures depicted in the frieze looks so startlingly vivid that you could almost imagine them coming to life and stepping out of the stucco.";

        Image = "images/filler1.png";

        Choices.clear();
        Choices.push_back(Choice::Base("Go over and talk to the priestess", 54));
        Choices.push_back(Choice::Base("Find a place to stay", 101, Choice::Type::MONEY, 1));
        Choices.push_back(Choice::Base("Seek an audience with the King of Nachan", 77));

        Controls = StandardControls();
    }
};

class Story435 : public Story::Base
{
public:
    Story435()
    {
        ID = 435;

        Text = "You cast a negation spell, causing the flames to peter out. Jade Thunder scrabbles forward across the sand and snatches up his wand with a great shout of joy. \"At last!\" he cries. \"Now let me show you what can be achieved by a Grand Adept of our most potent art.\"";

        Image = "images/filler1.png";

        Choices.clear();

        Controls = StandardControls();
    }

    int Continue(Character::Base &player) { return 91; }
};

class Story437 : public Story::Base
{
public:
    Story437()
    {
        ID = 437;

        Text = "You set sail and continue your voyage west.";

        Image = "images/filler1.png";

        Choices.clear();

        Controls = StandardControls();
    }

    int Background(Character::Base &player)
    {
        if (Character::VERIFY_CODEWORD(player, Codeword::Type::EB))
        {
            return 387;
        }
        else
        {
            return -1;
        }
    }

    int Continue(Character::Base &player) { return 300; }
};

class Story441 : public Story::Base
{
public:
    Story441()
    {
        ID = 441;

        Text = "You know that the terrain of the forest generally rises towards the south and west. On the basis of this, the direction of the woodland streams gives you at least a hint about which way to go to reach Nachan. You need to bear steadily left until you begin to see signs of human habitation.";

        Image = "images/filler1.png";

        Choices.clear();

        Controls = StandardControls();
    }

    int Continue(Character::Base &player) { return 326; }
};

class NotImplemented : public Story::Base
{
public:
    NotImplemented()
    {
        ID = -1;

        Title = "Not implemented yet";

        Controls.clear();
        Controls.push_back(Button(0, "icons/exit.png", 0, 0, -1, -1, (1.0 - Margin) * SCREEN_WIDTH - buttonw, buttony, Control::Type::BACK));
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
auto story003 = Story003();
auto story004 = Story004();
auto story005 = Story005();
auto story006 = Story006();
auto story007 = Story007();
auto story008 = Story008();
auto story009 = Story009();
auto story010 = Story010();
auto story011 = Story011();
auto story012 = Story012();
auto story013 = Story013();
auto story014 = Story014();
auto story015 = Story015();
auto story016 = Story016();
auto story017 = Story017();
auto story018 = Story018();
auto story019 = Story019();
auto story020 = Story020();
auto story021 = Story021();
auto story022 = Story022();
auto story023 = Story023();
auto story024 = Story024();
auto story025 = Story025();
auto story026 = Story026();
auto story027 = Story027();
auto story028 = Story028();
auto story029 = Story029();
auto story030 = Story030();
auto story031 = Story031();
auto story032 = Story032();
auto story033 = Story033();
auto story034 = Story034();
auto story035 = Story035();
auto story036 = Story036();
auto story037 = Story037();
auto story038 = Story036();
auto story039 = Story039();
auto story040 = Story040();
auto story041 = Story041();
auto story042 = Story042();
auto story043 = Story043();
auto story044 = Story044();
auto story045 = Story045();
auto story046 = Story046();
auto story047 = Story047();
auto story048 = Story048();
auto story049 = Story049();
auto story050 = Story050();
auto story051 = Story051();
auto story052 = Story052();
auto story053 = Story053();
auto story054 = Story054();
auto story055 = Story055();
auto story056 = Story056();
auto story057 = Story057();
auto story058 = Story058();
auto story059 = Story059();
auto story060 = Story060();
auto story061 = Story061();
auto story062 = Story062();
auto story063 = Story063();
auto story064 = Story064();
auto story065 = Story065();
auto story066 = Story066();
auto story067 = Story067();
auto story068 = Story068();
auto story069 = Story069();
auto story070 = Story070();
auto story071 = Story071();
auto story072 = Story072();
auto story073 = Story073();
auto story074 = Story074();
auto story075 = Story075();
auto story076 = Story076();
auto story077 = Story077();
auto story078 = Story078();
auto story079 = Story079();
auto story080 = Story080();
auto story081 = Story081();
auto story082 = Story082();
auto story083 = Story083();
auto story084 = Story084();
auto story085 = Story085();
auto story086 = Story086();
auto story087 = Story087();
auto story088 = Story088();
auto story089 = Story089();
auto story090 = Story090();
auto story091 = Story091();
auto story092 = Story092();
auto story093 = Story093();
auto story094 = Story094();
auto story095 = Story095();
auto story096 = Story096();
auto story097 = Story097();
auto story098 = Story098();
auto story099 = Story099();
auto story100 = Story100();
auto story101 = Story101();
auto story102 = Story102();
auto story103 = Story103();
auto story104 = Story104();
auto story105 = Story105();
auto story106 = Story106();
auto story107 = Story107();
auto story108 = Story108();
auto story109 = Story109();
auto story110 = Story110();
auto story111 = Story111();
auto story112 = Story112();
auto story113 = Story113();
auto story114 = Story114();
auto story115 = Story115();
auto story116 = Story116();
auto story117 = Story117();
auto story118 = Story118();
auto story119 = Story119();
auto story120 = Story120();
auto story121 = Story121();
auto story122 = Story122();
auto story123 = Story123();
auto story124 = Story124();
auto story125 = Story125();
auto story126 = Story126();
auto story127 = Story127();
auto story128 = Story128();
auto story129 = Story129();
auto story130 = Story130();
auto story131 = Story131();
auto story132 = Story132();
auto story133 = Story133();
auto story134 = Story134();
auto story135 = Story135();
auto story136 = Story136();
auto story137 = Story137();
auto story138 = Story138();
auto story139 = Story139();
auto story140 = Story140();
auto story141 = Story141();
auto story142 = Story142();
auto story143 = Story143();
auto story144 = Story144();
auto story145 = Story145();
auto story146 = Story146();
auto story147 = Story147();
auto story148 = Story148();
auto story149 = Story149();
auto story150 = Story150();
auto story151 = Story151();
auto story152 = Story152();
auto story153 = Story153();
auto story154 = Story154();
auto story155 = Story155();
auto story156 = Story156();
auto story157 = Story157();
auto story158 = Story158();
auto story159 = Story159();
auto story160 = Story160();
auto story161 = Story161();
auto story162 = Story162();
auto story163 = Story163();
auto story164 = Story164();
auto story165 = Story165();
auto story166 = Story166();
auto story167 = Story167();
auto story168 = Story168();
auto story169 = Story169();
auto story170 = Story170();
auto story171 = Story171();
auto story172 = Story172();
auto story173 = Story173();
auto story174 = Story174();
auto story175 = Story175();
auto story176 = Story176();
auto story177 = Story177();
auto story178 = Story178();
auto story179 = Story179();
auto story180 = Story180();
auto story181 = Story181();
auto story182 = Story182();
auto story183 = Story183();
auto story184 = Story184();
auto story185 = Story185();
auto story186 = Story186();
auto story187 = Story187();
auto story188 = Story188();
auto story189 = Story189();
auto story190 = Story190();
auto story191 = Story191();
auto story192 = Story192();
auto story193 = Story193();
auto story194 = Story194();
auto story195 = Story195();
auto story196 = Story196();
auto story197 = Story197();
auto story198 = Story198();
auto story199 = Story199();
auto story200 = Story200();
auto story201 = Story201();
auto story202 = Story202();
auto story203 = Story203();
auto story204 = Story204();
auto story205 = Story205();
auto story206 = Story206();
auto story207 = Story207();
auto story208 = Story208();
auto story209 = Story209();
auto story210 = Story210();
auto story211 = Story211();
auto story212 = Story212();
auto story213 = Story213();
auto story214 = Story214();
auto story215 = Story215();
auto story216 = Story216();
auto story217 = Story217();
auto story218 = Story218();
auto story219 = Story219();
auto story220 = Story220();
auto story221 = Story221();
auto story222 = Story222();
auto story223 = Story223();
auto story224 = Story224();
auto story225 = Story225();
auto story226 = Story226();
auto story227 = Story227();
auto story228 = Story228();
auto story229 = Story229();
auto story230 = Story230();
auto story231 = Story231();
auto story232 = Story232();
auto story233 = Story233();
auto story234 = Story234();
auto story235 = Story235();
auto story236 = Story236();
auto story237 = Story237();
auto story238 = Story238();
auto story239 = Story239();
auto story240 = Story240();
auto story241 = Story241();
auto story242 = Story242();
auto story243 = Story243();
auto story244 = Story244();
auto story245 = Story245();
auto story246 = Story246();
auto story247 = Story247();
auto story248 = Story248();
auto story249 = Story249();
auto story250 = Story250();
auto story251 = Story251();
auto story252 = Story252();
auto story253 = Story253();
auto story254 = Story254();
auto story255 = Story255();
auto story256 = Story256();
auto story257 = Story257();
auto story258 = Story258();
auto story259 = Story259();
auto story260 = Story260();
auto story261 = Story261();
auto story262 = Story262();
auto story263 = Story263();
auto story264 = Story264();
auto story265 = Story265();
auto story266 = Story266();
auto story267 = Story267();
auto story268 = Story268();
auto story269 = Story269();
auto story270 = Story270();
auto story271 = Story271();
auto story272 = Story272();
auto story273 = Story273();
auto story274 = Story274();
auto story275 = Story275();
auto story276 = Story276();
auto story277 = Story277();
auto story278 = Story278();
auto story279 = Story279();
auto story280 = Story280();
auto story281 = Story281();
auto story282 = Story282();
auto story283 = Story283();
auto story284 = Story284();
auto story285 = Story285();
auto story286 = Story286();
auto story287 = Story287();
auto story288 = Story288();
auto story289 = Story289();
auto story290 = Story290();
auto story291 = Story291();
auto story292 = Story292();
auto story293 = Story293();
auto story294 = Story294();
auto story295 = Story295();
auto story296 = Story296();
auto story297 = Story297();
auto story298 = Story298();
auto story299 = Story299();
auto story300 = Story300();
auto story301 = Story301();
auto story302 = Story302();
auto story303 = Story303();
auto story304 = Story304();
auto story305 = Story305();
auto story306 = Story306();
auto story307 = Story307();
auto story308 = Story308();
auto story309 = Story309();
auto story310 = Story310();
auto story311 = Story311();
auto story312 = Story312();
auto story313 = Story313();
auto story314 = Story314();
auto story315 = Story315();
auto story316 = Story316();
auto story317 = Story317();
auto story318 = Story318();
auto story319 = Story319();
auto story320 = Story320();
auto story321 = Story321();
auto story322 = Story322();
auto story323 = Story323();
auto story324 = Story324();
auto story325 = Story325();
auto story326 = Story326();
auto story327 = Story327();
auto story328 = Story328();
auto story329 = Story329();
auto story330 = Story330();
auto story331 = Story331();
auto story332 = Story332();
auto story333 = Story333();
auto story334 = Story334();
auto story335 = Story335();
auto story343 = Story343();
auto story344 = Story344();
auto story345 = Story345();
auto story346 = Story346();
auto story347 = Story347();
auto story350 = Story350();
auto story354 = Story354();
auto story355 = Story355();
auto story356 = Story356();
auto story357 = Story357();
auto story366 = Story366();
auto story367 = Story367();
auto story368 = Story368();
auto story369 = Story369();
auto story370 = Story370();
auto story371 = Story371();
auto story374 = Story374();
auto story378 = Story378();
auto story387 = Story387();
auto story388 = Story388();
auto story389 = Story389();
auto story390 = Story390();
auto story391 = Story391();
auto story392 = Story392();
auto story396 = Story396();
auto story398 = Story398();
auto story400 = Story400();
auto story406 = Story406();
auto story407 = Story407();
auto story408 = Story408();
auto story409 = Story409();
auto story411 = Story411();
auto story415 = Story415();
auto story416 = Story416();
auto story417 = Story417();
auto story424 = Story424();
auto story425 = Story425();
auto story426 = Story426();
auto story435 = Story435();
auto story437 = Story437();
auto story441 = Story441();

void InitializeStories()
{
    Stories = {
        &prologue, &story001, &story002, &story003, &story004, &story005, &story006, &story007, &story008, &story009,
        &story010, &story011, &story012, &story013, &story014, &story015, &story016, &story017, &story018, &story019,
        &story020, &story021, &story022, &story023, &story024, &story025, &story026, &story027, &story028, &story029,
        &story030, &story031, &story032, &story033, &story034, &story035, &story036, &story037, &story038, &story039,
        &story040, &story041, &story042, &story043, &story044, &story045, &story046, &story047, &story048, &story049,
        &story050, &story051, &story052, &story053, &story054, &story055, &story056, &story057, &story058, &story059,
        &story060, &story061, &story062, &story063, &story064, &story065, &story066, &story067, &story068, &story069,
        &story070, &story071, &story072, &story073, &story074, &story075, &story076, &story077, &story078, &story079,
        &story080, &story081, &story082, &story083, &story084, &story085, &story086, &story087, &story088, &story089,
        &story090, &story091, &story092, &story093, &story094, &story095, &story096, &story097, &story098, &story099,
        &story100, &story101, &story102, &story103, &story104, &story105, &story106, &story107, &story108, &story109,
        &story110, &story111, &story112, &story113, &story114, &story115, &story116, &story117, &story118, &story119,
        &story120, &story121, &story122, &story123, &story124, &story125, &story126, &story127, &story128, &story129,
        &story130, &story131, &story132, &story133, &story134, &story135, &story136, &story137, &story138, &story139,
        &story140, &story141, &story142, &story143, &story144, &story145, &story146, &story147, &story148, &story149,
        &story150, &story151, &story152, &story153, &story154, &story155, &story156, &story157, &story158, &story159,
        &story160, &story161, &story162, &story163, &story164, &story165, &story166, &story167, &story168, &story169,
        &story170, &story171, &story172, &story173, &story174, &story175, &story176, &story177, &story178, &story179,
        &story180, &story181, &story182, &story183, &story184, &story185, &story186, &story187, &story188, &story189,
        &story190, &story191, &story192, &story193, &story194, &story195, &story196, &story197, &story198, &story199,
        &story200, &story201, &story202, &story203, &story204, &story205, &story206, &story207, &story208, &story209,
        &story210, &story211, &story212, &story213, &story214, &story215, &story216, &story217, &story218, &story219,
        &story220, &story221, &story222, &story223, &story224, &story225, &story226, &story227, &story228, &story229,
        &story230, &story231, &story232, &story233, &story234, &story235, &story236, &story237, &story238, &story239,
        &story240, &story241, &story242, &story243, &story244, &story245, &story246, &story247, &story248, &story249,
        &story250, &story251, &story252, &story253, &story254, &story255, &story256, &story257, &story258, &story259,
        &story260, &story261, &story262, &story263, &story264, &story265, &story266, &story267, &story268, &story269,
        &story270, &story271, &story272, &story273, &story274, &story275, &story276, &story277, &story278, &story279,
        &story280, &story281, &story282, &story283, &story284, &story285, &story286, &story287, &story288, &story289,
        &story290, &story291, &story292, &story293, &story294, &story295, &story296, &story297, &story298, &story299,
        &story300, &story301, &story302, &story303, &story304, &story305, &story306, &story307, &story308, &story309,
        &story310, &story311, &story312, &story313, &story314, &story315, &story316, &story317, &story318, &story319,
        &story320, &story321, &story322, &story323, &story324, &story325, &story326, &story327, &story328, &story329,
        &story330, &story331, &story332, &story333, &story334, &story335,
        &story343, &story344, &story345, &story346, &story347,
        &story350, &story354, &story355, &story356, &story357,
        &story366, &story367, &story368, &story369,
        &story370, &story371, &story374, &story378,
        &story387, &story388, &story389,
        &story390, &story391, &story392, &story398,
        &story400, &story406, &story407, &story408, &story409,
        &story411, &story415, &story416, &story417,
        &story424, &story425, &story426,
        &story435, &story437,
        &story441};
}

#endif