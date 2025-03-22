# PROJECT-MBTA

PREREQUISITES:
The meat and potatoes of this project requires a decent understanding of the Arduino platform, how to read hexadecimal, and a basic understanding of C++.
I will not be fully explaining every detail of how this works, as I am assuming decent proficency in these areas. Addtionally you may notice different parts of the code provided are redacted. This is because I do not condone using this guide as a means of stealing from the MBTA, rather as an educational repository to show the dire changes the City of Boston has been neglecting to implement into their T card system (and more broadly into the MBTA as a whole). I do not condone stealing, and I reccomend if you have found this repository in hopes of doging you're fare to the MBTA to kindly go fuck yourself and get of my page.

INTRODUCTION:
The MBTA is the public transporation system in the city of Boston. Since 2006 the MBTA has used the MIFARE-CLASSIC T-Cards as the primary method of payment for using the T in the city of Boston. All trains, buses ect. use this payment system and continue to use it today. The city of Boston has neglected to update this system and properly fund the MBTA leading to a stagnation in the technology and infastructure of the MBTA. 
Nearly a million Bostonians ride the MBTA every week, and for those of lower economic status it may be there only form of transporation (as Boston is one of the most expensive cities to live in). The MBTA's lack of funding has caused trains and buses to be less frequent and predicatable, more likely to break down, and has caused multiple incidents where trains were shut down for weeks or months. According to healthaffairs.org "Lack of access to public transportation can disproportionately harm older people and people with disabilities. It can also contribute to existing racial and economic disparities by decreasing mobility and forcing individuals to depend on costly car ownership"(Heaps et al. paragraph 5). This project seeks to demonstrate this lack of care by showing how easily exploitable this system is, and the MBTA's dire need for greater techonlogical security.

**Chapter 1: The MBTA Charlie Card**

<img src="https://github.com/user-attachments/assets/b1e51d0b-8cd5-4c20-b9b0-0d7f43c04925" alt="charliecard" width="200" style="float: left; margin-right: 10px;"/>

The MBTA Charlie Card was first introduced in 2006 and is a Mifare Classic card developed by NXP semi conductors. At most underground T stations there are kiosks that allow indivduals to check the balance of their card, and upload money to it via cash, debit card, or credit card. At virtually every MBTA station these cards are accepted with the user simply having to tap their card to pay their fare. Mifare technology was developed in 1994 and nearly 30 years later is a technology showing severe age. This project will exploit this aging technology allowing us to physically change the amount of money on the card.

**Chapter 2: How does Mifare Classic Work?**
Mifare classic cards are a form of NFC (near field communication) that holds particular data within itself. In the case of the MBTA these cards are the Mifare Classic 1k cards (1 kilobyte or 1000 bytes of data). Each card is divided into 16 different sectors containing 4 block each (totalling 64 blocks). The fourth block of each sector contains the keys for that particular block allowing for the data within them to read properly. These keys are part of the cryto-1 system, a basic form of crytography which keeps the blocks from being read by a standard NFC reader. Additionally each card contains a unique HWID that cannot be changed, and can be found in the first 5 bytes of the card.
Below is an example of what a single sector on a charlie card looks like:

<img src="https://github.com/user-attachments/assets/5419744c-09ce-4d37-8514-b35ee56e9730" alt="ex Sector" width="300"/>


**Chapter 3: Materials and Basics of reading NFC cards**

The materials needed for this project are going to be a PN-532 NFC reader, a UART I2C adapter (if your a windows user) and a Arduino UNO. I will also highly reccomend having a windows or a linux machine as there will be necesssary software later that will be needed to interact with the NFC reader.

This website details everything that is needed in setting up the PN532 with the arduino including a library for interacting with it:
https://www.elechouse.com/elechouse/images/product/PN532_module_V3/PN532_%20Manual_V3.pdf

The library once installed in you're Arduino IDE includes examples of how to interact with the board, including reading the HWID and writing to blocks within the card.
Important Functions: 
readPassiveTargetID -- To obtain the UID of the card
mifareclassic_ReadDataBlock -- to read a selected block of the mifare classic card
mifareclassic_AuthenticateBlock -- to authenticate a certain block with a given to gain access to read and write privledged to that block




