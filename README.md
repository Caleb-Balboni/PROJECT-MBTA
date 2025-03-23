# PROJECT-MBTA

# PREREQUISITES:
The meat and potatoes of this project requires a decent understanding of the Arduino platform, how to read hexadecimal, and a basic understanding of C++.
I will not be fully explaining every detail of how this works, as I am assuming decent proficency in these areas. Addtionally you may notice different parts of the code provided are redacted. This is because I do not condone using this guide as a means of stealing from the MBTA, rather as an educational repository to show the dire changes the City of Boston has been neglecting to implement into their T card system (and more broadly into the MBTA as a whole). I do not condone stealing, and I reccomend if you have found this repository in hopes of doging you're fare to the MBTA to kindly go fuck yourself and get of my page.

# INTRODUCTION:
The MBTA is the public transporation system in the city of Boston. Since 2006 the MBTA has used the MIFARE-CLASSIC T-Cards as the primary method of payment for using the T in the city of Boston. All trains, buses ect. use this payment system and continue to use it today. The city of Boston has neglected to update this system and properly fund the MBTA leading to a stagnation in the technology and infastructure of the MBTA. 
Nearly a million Bostonians ride the MBTA every week, and for those of lower economic status it may be there only form of transporation (as Boston is one of the most expensive cities to live in). The MBTA's lack of funding has caused trains and buses to be less frequent and predicatable, more likely to break down, and has caused multiple incidents where trains were shut down for weeks or months. According to healthaffairs.org "Lack of access to public transportation can disproportionately harm older people and people with disabilities. It can also contribute to existing racial and economic disparities by decreasing mobility and forcing individuals to depend on costly car ownership"(Heaps et al. paragraph 5). This project seeks to demonstrate this lack of care by showing how easily exploitable this system is, and the MBTA's dire need for greater techonlogical security. To exemplify these weaknesses, this guide will show how easy is it to hack the MBTA charlie card, and edit the amount of money on the card.

## Chapter 1: The MBTA Charlie Card

<img src="https://github.com/user-attachments/assets/b1e51d0b-8cd5-4c20-b9b0-0d7f43c04925" alt="charliecard" width="200" style="float: left; margin-right: 10px;"/>

The MBTA Charlie Card was first introduced in 2006 and is a Mifare Classic card developed by NXP semi conductors. At most underground T stations there are kiosks that allow indivduals to check the balance of their card, and upload money to it via cash, debit card, or credit card. At virtually every MBTA station these cards are accepted with the user simply having to tap their card to pay their fare. Mifare technology was developed in 1994 and nearly 30 years later is a technology showing severe age. This project will exploit this aging technology allowing us to physically change the amount of money on the card.

## Chapter 2: How does Mifare Classic Work?
Mifare classic cards are a form of NFC (near field communication) that holds particular data within itself. In the case of the MBTA these cards are the Mifare Classic 1k cards (1 kilobyte or 1000 bytes of data). Each card is divided into 16 different sectors containing 4 block each (totalling 64 blocks). The fourth block of each sector contains the keys for that particular block allowing for the data within them to read properly. These keys are part of the cryto-1 system, a basic form of crytography which keeps the blocks from being read by a standard NFC reader. Additionally each card contains a unique HWID that cannot be changed, and can be found in the first 5 bytes of the card.
Below is an example of what a single sector on a charlie card looks like:

<img src="https://github.com/user-attachments/assets/5419744c-09ce-4d37-8514-b35ee56e9730" alt="ex Sector" width="300"/>


## Chapter 3: Materials and Basics of Reading NFC Cards


For this project, you'll need:

- **PN-532 NFC Reader**
- **UART/I2C Adapter** (recommended for Windows users)
- **Arduino UNO**
- A computer running **Windows** or **Linux** (Mac may work, but instructions are optimized for Windows/Linux)

I highly recommend using Windows or Linux, as some of the necessary software tools used later to interact with the NFC reader work best on those systems.


To get started with the PN-532 module and Arduino, refer to the official setup guide provided here:

🔗 [PN532 Module Manual & Setup Instructions](https://www.elechouse.com/elechouse/images/product/PN532_module_V3/PN532_%20Manual_V3.pdf)

This includes:

- Wiring diagrams
- Library installation
- Example sketches for reading cards
- How to write to specific memory blocks

Once the library is installed in your Arduino IDE, it includes examples to help you:

- Read the **UID** (Unique Identifier) of a card
- Authenticate specific blocks
- Read from and write to MIFARE Classic cards


### Important Functions

Here are some key functions you’ll use from the library:

```cpp
// Reads the UID (Unique Identifier) of the card
readPassiveTargetID()

// Authenticates a specific block using a key
mifareclassic_AuthenticateBlock()

// Reads data from a specified block (after authentication)
mifareclassic_ReadDataBlock()
```

## Chapter 4: Cracking Mifare keys

Since 2009 there has been publicly available exploits for the crypto-1 security system of the Mifare Classic card. Multiple tools are publicly available for this process with the most notable being MFOC and MFCUK. I will not be detailing exactly how to setup and use these tools as plenty of public documentation exists already, but I will give a brief overview of the basic steps of setting up these tools with your windows machine.

**Setup on Windows**
1. Install a Linux vm (I used Kali Linux)
2. Use a UART adapter to connect your PN532 to your windows machine via USB
3. use wsl to share the UART connection with your linux vm
4. install MFOC within your linux vm
5. crack the keys
(for the sake of safety I have left out a vital step necessary to crack the keys. You can do your own research to figure out what this may mean)

## Chapter 5: Editing values within the card

Now that we have obtained the keys and cracked the card, you will most likely have a dump of the decyphered card. This is where we will exploit the MBTA's most critical flaw being the amount of money a card has is stored not on a secure data base but on the card directly. Yes you heard me correctly, the truth for how much money the card currently has is directly on the card, and there is not check to any database as to if this value has been manipulated. Yet there is still a major interference that stands in our way, the checksum. The checksum is used to ensure that any data in the line before it has not been tampered with, and is a specially created algorithm making it so every card will have a unique checksum even on a identical line. I am not aware of any source that has cracked this checksum algorithm, and even with many hours of trial and error I failed as well.

**Generating a new checksum**

From my online research I found that when two cards seperate and unique cards are used in tandem there is a way to copy lines from one card to another and generate the correct checksum.

Say for instance I have the lines
```cpp
// The first card
00 20 00 00 00 00 00 00 20 00 00 00 00 00 c3 78
// The second card
00 20 00 00 00 00 00 00 20 00 00 00 00 00 77 d1
```
The checksums for these cards are C378 and 77D1. If we XOR these checksums together we get B4A9 this is what we can call the checksum modifier, and is going to be vital for copying money values from one card to the next. All one must do to copy a line from one card to another is write the line from the first card into the second card, and XOR the checksum of this new line by the checksum modifer. This will create a new valid line on the new card, as if it had been there all along. So for example say one knows the line where the money value is stored on one of these MBTA cards. I could simply add 50 dollars to one card, copy that line to the new card, XOR that value by the checksum modifer, and boom we have a second card with 50 dollars as well.

**In Summary**
1. Find two identical line on two serpate cards
2. XOR their checksums together to get the checksum modifer
3. copy the original line to the new card
4. XOR the this new line by the checksum modifer
5. The line has now been succesfully copied

## Chapter 5: Explanation of the provided code

The code I wrote is extrodiarily simply and automates the steps above. First we read the data from a known block that will be identical to a block on the original card (with money on it). We can than XOR the checksum of the read block (dataToRead[14] and dataToRead[15] are the 15 and 16th bytes of this block), with the same checksum from the original card (stored in originalChecksum[2] for convience) this gives us the data modifer. Than we can XOR this data modifer with the checksum of line that is going to be copied (saved as dataToWrite[16]) which will produce a new line that is read to by copied to the new card.

**What this Code looks like**
```cpp
if (nfc.mifareclassic_ReadDataBlock(REDACTED, dataToRead)) { // authenticate a known block that will have an identical value the known card
    uint8_t checkSumMod1 = originalChecksum[0] ^ dataToRead[14]; // XOR the checksums of the known checksum of the block to be written with the read checksum data
    uint8_t checkSumMod2 = originalChecksum[1] ^ dataToRead[15]; // XOR the checksums of the known checksum of the block to be written with the read checksum data
    dataToWrite[14] = REDACTED ^ checkSumMod1; // XOR the checksum modifer with the the cheksum of the data to be written
    dataToWrite[15] = REDACTED ^ checkSumMod2; // XOR the checksum modifer with the the cheksum of the data to be written

// Now we have a new line that can be written to the new card with the proper checksum
```

Next we can than write this new line to where to a new line on card which looks something like this

```cpp
if (nfc.mifareclassic_AuthenticateBlock(uid, uidLength, REDACTED, 1, keya)) { // Authenticate block to be written to
     if(nfc.mifareclassic_WriteDataBlock(9, dataToWrite)) {} // Write to that particular block
```
If you notice I actually write two seperate times to two seperate blocks, I am going to leave this reasoning ambiguous, and is something you should figure out on your own.



