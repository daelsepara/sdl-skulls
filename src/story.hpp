#ifndef __STORY__HPP__
#define __STORY__HPP__

#include <vector>
#include "constants.hpp"
#include "controls.hpp"
#include "items.hpp"
#include "skills.hpp"

enum class ChoiceType
{
    NORMAL = 0, // No requirements
    ITEM,
    SKILL,
    CODEWORD,
    MONEY,
    LIFE,
    ANY_ITEM,
    ANY_SKILL,
    SKILL_ITEM,
    FIRE_WEAPON,
    LOSE_ITEM,
    LOSE_LIFE
};

class Choice
{
public:
    ChoiceType Type = ChoiceType::NORMAL;
    const char *Text = NULL;
    ItemType Item = ItemType::ANY;
    SkillType Skill = SkillType::NONE;
    int Value = 0;
    int Destination = -1;

    Choice(ChoiceType type, const char *text, int destination, ItemType item, SkillType skill, int value)
    {
        Type = type;
        Text = text;
        Destination = destination;
        Item = item;
        Skill = skill;
        Value = value;
    }

    Choice(const char *text, int destination, ItemType item, SkillType skill)
    {
        Text = text;
        Destination = destination;
        Type = ChoiceType::SKILL_ITEM;
        Item = item;
        Skill = skill;
    }

    Choice(const char *text, int destination, ItemType item)
    {
        Text = text;
        Destination = destination;
        Type = ChoiceType::ITEM;
        Item = item;
    }

    Choice(const char *text, int destination, SkillType skill)
    {
        Text = text;
        Destination = destination;
        Type = ChoiceType::SKILL;
        Skill = skill;
    }

    Choice(const char *text, int destination, ChoiceType type, int value)
    {
        Text = text;
        Destination = destination;
        Type = type;
        Value = value;
    }

    Choice(const char *text, int destination, ChoiceType type)
    {
        Text = text;
        Destination = destination;
        Type = type;
    }
};

enum class StoryType
{
    NORMAL = 0,
    UNCERTAIN,
    GOOD,
    BAD
};

class Story
{
public:
    int ID = 0;
    const char *Text = NULL;
    const char *Title = NULL;
    const char *Image = NULL;
    std::vector<Button> Controls;
    std::vector<Choice> Choices;
    StoryType Type = StoryType::NORMAL;

    // Handle background events
    virtual int Background() { return -1; };

    // Handle events before story branches
    virtual int Event() { return -1; };

    // Jump to next section
    virtual int Continue() { return -1; };

    Story()
    {
    }

    Story(int id)
    {
        ID = id;
    }

    Story(int id, const char *text, StoryType type)
    {
        ID = id;
        Text = text;
        type = type;
    }

    Story(const char *text, StoryType type)
    {
        Text = text;
        type = type;
    }
};

class Prologue : public Story
{
public:
    Prologue()
    {
        ID = 0;
        Title = "Necklace of Skulls: Prologue";
        Text = "Last night you dreamed you saw your brother again. He was walking through a desert, his sandals scuffing up plumes of sooty black sand from the low endless dunes. It seemed you were hurrying to catch him up, but the sand slipped away under your feet and you could make no headway up the slope. You heard your own voice call his name: \"Morning Star!\" But muffled by distance, the words went rolling off the sky unheeded.\n\nYou struggled on. Cresting the dune, you saw your brother standing close by, staring at something in his hands. Your heart thudded with relief as you stumbled through the dream towards him. But even as your hand reached out for his shoulder, a sense of dread was growing like a storm cloud to blot out any joy. You saw the object Morning Star was holding: an obsidian mirror. You leaned forward and gazed at the face of your brother reflected in the dark green glass.\n\nYour twin brother's face was the face of a skull.\n\nThe soothsayer nods as you finish recounting the dream. He plays idly with his carved stone prophecy-markers, pouring them from one hand to the other with a light rattling sound.\n\n\"Today is the day of Lamat,\" he says in his thin old voice. \"And the symbol of Lamat is the death's head. On this day, the morning-star has ended its cycle and will not be visible in the heavens for ninety days, when it will reappear as the evening-star. The meaning of the dream is therefore that in the absence of your brother it falls to you, Evening Star to fulfil his duties.\"\n\nYou cannot resist a smile, even though the ominous import of the dream weighs heavily on your soul. \"So it only concerns the importance of duty? I wonder if my clan elders have been speaking to you?\"\n\nThe soothsayer snorts and casts the prophecy-markers back into his bag with a pretence of indignation, but he has too good a heart to overlook your concern for your brother. Turning at the door, he adds, \"The King gave Morning Star a great honour when he made him his ambassador. But it is no less honourable to stay at home and help with the affairs of one's clan. You are young, Evening Star; your chance for glory will come.\"\n\n\"Do the prophecy-makers also tell you that?\"\n\nHe rattles the bag. \"These? They're just for show; it's the two old stones on either side of my nose that tell me everything I need to know about the future!\" He points to his eyes and hobbles out in a gale of wheezing laughter.\n\nYou lean back, feeling the cool of the stone wall press against your bare shoulders. The soothsayer intended to set your mind at rest, but you have shared a bond with Morning Star since the two of you were born. To be troubled by such a dream is not, you feel sure, a mere quirk of the imagination. Somehow you sense that something terrible has befallen your brother.\n\nYou are still brooding an hour later when a servant comes scurrying into the room. \"There is news of Lord Morning Star's expedition...\" he begins, almost too frightened of your reaction to blurt out the words.\n\nYou are on your feet in an instant. \"What news?\"\n\nThe servant bows. \"The Council of Nobles is holding an emergency session. The rumour... I have heard a rumour that only a single member of the expedition returned alive.\"\n\nPausing only to draw on your cloak, you hurry outside and head along the street towards the city centre. All around you sprawl the tall thatched roofs of the city, spreading out towards the distant fields. Each clan or group of families has its own dwellings of stone or mud-brick, according to status. These rest upon raised platforms above the level of the street, their height determined again by status. But not even the most exalted noble has a home to match the grand dwellings of the gods, which you now see towering ahead of you atop their immense pyramids. They shimmer with the colours of fresh blood and polished bone in the noonday sunlight, covered with demonic carvings which stare endlessly down across the city of Koba.\n\nThe central plaza of the city is a blaze of white stone in the sunshine. Quickening your step, you approach the amphitheatre where the Council of Nobles is meeting. As you step under the arch of the entrance, your way is barred by two burly warriors of the King's guard, each armed with an obsidian-edged sword. \"You may not enter.\"\n\n\"I am Evening Star. The ambassador is my brother. Has he returned to Koba?\"\n\nOne of them peers at you, recognition trickling like cold honey into his gaze. \"I know you now. Morning Star has not returned, no.\"\n\nThe other says, \"Look, I suppose you'd better go in. One of the ambassador's retinue came back this morning. He's telling the Council what happened.\"\n\nYou walk in to the amphitheatre and numbly find a seat. You can hardly take in the guard's words; they sit like stones in your head, impossible to accept. Can it be true? Your twin brother -- dead?\n\nA man you recognize as one of Morning Star's veteran warriors stands in the centre of the amphitheatre, giving his report. The seats on either side are crowded with the lords and ladies of Koba, each face a picture of grave deliberation. At the far end is the King himself, resplendent in a turquoise mantle of quetzal feathers, his throne carved to resemble the open jaws of some titanic monster on whose tongue he seems to sit like the very decree of the gods.\n\n\"...arrived at the Great City,\" the veteran is saying. \"We found it ransacked -- the temples torn down, whole palaces burned. Some poor wretches still live there, eking out a stark existence in the ruins, but it is like the carcass of a beast who lies with a death-wound. Whenever we asked how this destruction had come about, we received the same reply: werewolves from the land of the dead, beyond the west, had descended from the desert and slain all the Great City's defenders in a single night of carnage.\"\n\nThere is a murmuring at this. The Great City had endured for centuries before Koba was even built. The King raises his hand for silence. \"What was Lord Morning Star's decision when he heard this?\"\n\n\"Majesty, he led us into the desert. He believed it his duty to uncover the truth of the matter and report it to you. After many days of trekking almost all our water was used up. We had faced monsters along the way, and many of us bore grievous injuries. Then we came to a place like a royal palace, but entirely deserted except for dogs and owls. We camped outside the walls there, and on the next day Morning Star told us he had dreamed of a sorcerer called Necklace of Skulls who dwelt within the palace. He said he would enter and find out if this sorcerer had sent the werewolves to destroy the Great City. We watched him enter the portals of the palace, and we waited for his return for eight days, but he did not emerge. Then we began the long march back here to Koba, but sickness and the creatures of the desert gradually took their toll, and I alone remain to tell the tale.\"\n\nThe King rises to his feet. \"Morning Star must be considered slain by this sorcerer. His mission shall not be recorded as a failure, since he died attempting to carry out his duty. Prayers shall be said for the safe journey of his soul through the underworld. This meeting is ended.\"\n\nThe others file out in groups, heads bent together in urgent debate. For most of them the veteran's report carried that special thrill of a distant alarm. A great but far-off city reduced to ruin; a disaster from halfway across the world. Cataclysmic news, but an event comfortingly remote from the day to day affairs at home. A matter for the noblemen to worry over when they sit with their cigars at night. The reverberations of a toppling temple in the Great City will be heard here in Koba as no more than the droning discussions of old men.\n\nFor you it is different. Left alone in the amphitheatre, you sit like a figure of clay, eerily detached from your own turbulent emotions. Fractured images and words whirl through your stunned brain. Morning Star is dead. Your twin brother, lost for ever...\n\nA single sudden thought of burning clarity impels you to your feet. In that instant you seem to see your destiny unrolling in front of you like along straight carpet. You turn your face to the west, eyes narrowing in the glare of the declining sun.\n\nYour brother might not be dead. There is only one place you can learn the truth.\n\nYou must journey to the western desert, to the palace of the sorcerer called Necklace of Skulls.";

        Image = "images/filler1.png";

        Choices.clear();

        Controls.clear();
        Controls.push_back(Button(0, "images/up-arrow.png", 0, 1, 0, 1, (1 - Margin) * SCREEN_WIDTH - arrow_size, texty + border_space, ControlType::SCROLL_UP));
        Controls.push_back(Button(1, "images/down-arrow.png", 0, 2, 0, 2, (1 - Margin) * SCREEN_WIDTH - arrow_size, texty + text_bounds - arrow_size - border_space, ControlType::SCROLL_DOWN));
        Controls.push_back(Button(2, "images/map.png", 1, 3, 1, 2, startx, buttony, ControlType::MAP));
        Controls.push_back(Button(3, "images/disk.png", 2, 4, 1, 3, startx + gridsize, buttony, ControlType::GAME));
        Controls.push_back(Button(4, "images/next.png", 3, 5, 1, 4, startx + 2 * gridsize, buttony, ControlType::NEXT));
        Controls.push_back(Button(5, "images/exit.png", 4, 5, 1, 5, (1 - Margin) * SCREEN_WIDTH - buttonw, buttony, ControlType::QUIT));
    }

    int Continue() { return 1; };
};

class Story01 : public Story
{
public:
    Story01()
    {
        ID = 1;
        Title = "Necklace of Skulls: 001";
        Text = "Seeking an audience with the Matriarch of your clan, you are shown into a narrow steep-vaulted hall. Sunlight burns through the high window slits to leave hovering blocks of dazzling yellow light on the whitewashed wall, but the room is cool.\n\nThe Matriarch sits cross-legged on a stone bench at the end of the room, below a large painted glyph which is the symbol of the clan. A stout woman in late middle-age, she has a soft and even jolly appearance which is belied by the look of stern contemplation in her eyes. The beads sewn across here cotton mantle make a rustling sound as she waves you towards a straw mat. You bow in greeting before sitting, and a servant brings you a cup of frothy peppered cocoa.\n\nThe Matriarch fixes you with her glass-bead gaze. \"Evening Star, I understand you wish to leave Koba and travel in search of your brother.\"\n\n\"I must learn what has happened to him, my lady. If he is alive, perhaps I can rescue him; if dead, it is my duty to avenge him.\"\n\nThe Matriarch folds her fat jade-ringed fingers and rests her chin on them, watching you as though weighing the worth of your soul. \"You speak of duty,\" she says. \"Have you no duty to your clan here in Koba? Does honour demand that we lose another scion in pursuit of a hopeless quest?\"\n\nYou sip cocoa while considering your next words carefully.";
        Image = "images/filler1.png";

        Choices.clear();
        Choices.push_back(Choice("Reply that the life of your brother is more important than your duty to the clan", -1, ChoiceType::NORMAL));
        Choices.push_back(Choice("...that on the contrary, clan honour demands that you go", -1, ChoiceType::NORMAL));
        Choices.push_back(Choice("Say nothing", -1, ChoiceType::NORMAL));

        Controls.clear();
        Controls.push_back(Button(0, "images/up-arrow.png", 0, 1, 0, 1, (1 - Margin) * SCREEN_WIDTH - arrow_size, texty + border_space, ControlType::SCROLL_UP));
        Controls.push_back(Button(1, "images/down-arrow.png", 0, 2, 0, 2, (1 - Margin) * SCREEN_WIDTH - arrow_size, texty + text_bounds - arrow_size - border_space, ControlType::SCROLL_DOWN));
        Controls.push_back(Button(2, "images/map.png", 1, 3, 1, 2, startx, buttony, ControlType::MAP));
        Controls.push_back(Button(3, "images/disk.png", 2, 4, 1, 3, startx + gridsize, buttony, ControlType::GAME));
        Controls.push_back(Button(4, "images/next.png", 3, 5, 1, 4, startx + 2 * gridsize, buttony, ControlType::NEXT));
        Controls.push_back(Button(5, "images/exit.png", 4, 5, 1, 5, (1 - Margin) * SCREEN_WIDTH - buttonw, buttony, ControlType::QUIT));
    }
};

class NotImplemented : public Story
{
public:
    NotImplemented()
    {
        ID = -1;
        Title = "Not implemented yet";

        Controls.clear();
        Controls.push_back(Button(0, "images/exit.png", 0, 0, -1, -1, (1 - Margin) * SCREEN_WIDTH - buttonw, buttony, ControlType::QUIT));
    }
};

auto notImplemented = NotImplemented();
auto prologue = Prologue();
auto story01 = Story01();

auto Stories = std::vector<Story *>();

void InitializeStories()
{
    Stories.push_back(&prologue);
    Stories.push_back(&story01);
}

void *findStory(int id)
{
    Story *story = &notImplemented;

    if (Stories.size() > 0)
    {
        for (auto i = 0; i < Stories.size(); i++)
        {
            if (((Story *)Stories[i])->ID == id)
            {
                story = (Story *)Stories[id];

                break;
            }
        }
    }

    return story;
}

#endif
