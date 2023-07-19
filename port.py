#!/usr/bin/env python3

# Heavily based on MKW-SP's port.py, changed output format

from argparse import ArgumentParser
from dataclasses import dataclass
import sys
from typing import Dict, List


@dataclass
class Section:
    name: str
    start: int
    end: int

    def __contains__(self, address):
        return self.start <= address < self.end


@dataclass
class SrcBinary:
    start: Dict[str, int]
    sections: List[Section]

    def __contains__(self, address):
        return any(address in section for section in self.sections)


@dataclass
class DstBinary:
    start: int
    end: int


@dataclass
class Chunk:
    src_start: int
    src_end: int
    dst_start: int

    def __contains__(self, address):
        return self.src_start <= address < self.src_end

    def port(self, address):
        return address - self.src_start + self.dst_start


SRC_BINARIES = {
    "P": {
        "dol": SrcBinary(
            0x80004000,
            [
                Section("init", 0x80004000, 0x80006460),
                Section("extab", 0x80006460, 0x80006A20),
                Section("extabindex", 0x80006A20, 0x800072C0),
                Section("text", 0x800072C0, 0x80244DE0),
                Section("ctors", 0x80244DE0, 0x80244EA0),
                Section("dtors", 0x80244EA0, 0x80244EC0),
                Section("rodata", 0x80244EC0, 0x80258580),
                Section("data", 0x80258580, 0x802A4040),
                Section("bss", 0x802A4080, 0x80384C00),
                Section("sdata", 0x80384C00, 0x80385FC0),
                Section("sbss", 0x80385FC0, 0x80386FA0),
                Section("sdata2", 0x80386FA0, 0x80389140),
                Section("sbss2", 0x80389140, 0x8038917C),
            ],
        ),
        "rel": SrcBinary(
            0x805102E0,
            [
                Section("text", 0x805103B4, 0x8088F400),
                Section("ctors", 0x8088F400, 0x8088F704),
                Section("dtors", 0x8088F704, 0x8088F710),
                Section("rodata", 0x8088F720, 0x808B2BD0),
                Section("data", 0x808B2BD0, 0x808DD3D4),
                Section("bss", 0x809BD6E0, 0x809C4F90),
            ],
        ),
    },
    "E": {
        "dol": SrcBinary(
            0x80004000,
            [
                Section("init", 0x80004000, 0x80006460),
                Section("extab", 0x80006460, 0x80006A20),
                Section("extabindex", 0x80006A20, 0x800072C0),
                Section("text", 0x800072C0, 0x80244D40),
                Section("ctors", 0x80244D40, 0x80244E00),
                Section("dtors", 0x80244E00, 0x80244E20),
                Section("rodata", 0x80244E40, 0x80258260),
                Section("data", 0x80258260, 0x8029FD00),
                Section("bss", 0x8029FD00, 0x80380880),
                Section("sdata", 0x80380880, 0x80381C40),
                Section("sbss", 0x80381C40, 0x80382C20),
                Section("sdata2", 0x80382C20, 0x80384DC0),
                Section("sbss2", 0x80384DC0, 0x80384DFC),
            ],
        ),
        "rel": SrcBinary(
            0x8050BF60,
            [
                Section("text", 0x8050C034, 0x8088AFD0),
                Section("ctors", 0x8088AFD0, 0x8088B2D4),
                Section("dtors", 0x8088B2D4, 0x8088B2E0),
                Section("rodata", 0x8088B2E0, 0x808AE520),
                Section("data", 0x808AE520, 0x808D8C7C),
                Section("bss", 0x809B8F20, 0x809C07D0),
            ],
        ),
    },
    "J": {
        "dol": SrcBinary(
            0x80004000,
            [
                Section("init", 0x80004000, 0x80006460),
                Section("extab", 0x80006460, 0x80006A20),
                Section("extabindex", 0x80006A20, 0x800072C0),
                Section("text", 0x800072C0, 0x80244D00),
                Section("ctors", 0x80244D00, 0x80244DC0),
                Section("dtors", 0x80244DC0, 0x80244DE0),
                Section("rodata", 0x80244E00, 0x80257F20),
                Section("data", 0x80257F20, 0x802A39E0),
                Section("bss", 0x802A3A00, 0x80384580),
                Section("sdata", 0x80384580, 0x80385940),
                Section("sbss", 0x80385940, 0x80386920),
                Section("sdata2", 0x80386920, 0x80388AC0),
                Section("sbss2", 0x80388AC0, 0x80388AFC),
            ],
        ),
        "rel": SrcBinary(
            0x8050FC60,
            [
                Section("text", 0x8050FD34, 0x8088EA6C),
                Section("ctors", 0x8088EA6C, 0x8088ED70),
                Section("dtors", 0x8088ED70, 0x8088ED7C),
                Section("rodata", 0x8088ED80, 0x808B1D30),
                Section("data", 0x808B1D30, 0x808DC524),
                Section("bss", 0x809BC740, 0x809C3FF0),
            ],
        ),
    },
    "K": {
        "dol": SrcBinary(
            0x80004000,
            [
                Section("init", 0x80004000, 0x80006460),
                Section("extab", 0x80006460, 0x80006A20),
                Section("extabindex", 0x80006A20, 0x800072C0),
                Section("text", 0x800072C0, 0x80245160),
                Section("ctors", 0x80245160, 0x80245220),
                Section("dtors", 0x80245220, 0x80245240),
                Section("rodata", 0x80245240, 0x80258340),
                Section("data", 0x80258340, 0x80292040),
                Section("bss", 0x80292080, 0x80372C00),
                Section("sdata", 0x80372C00, 0x80373FE0),
                Section("sbss", 0x80373FE0, 0x80374FC0),
                Section("sdata2", 0x80374FC0, 0x80377160),
                Section("sbss2", 0x80377160, 0x8037719C),
            ],
        ),
        "rel": SrcBinary(
            0x804FE300,
            [
                Section("text", 0x804FE3D4, 0x8087D7C0),
                Section("ctors", 0x8087D7C0, 0x8087DAC4),
                Section("dtors", 0x8087DAC4, 0x8087DAD0),
                Section("rodata", 0x8087DAE0, 0x808A1030),
                Section("data", 0x808A1030, 0x808CB86C),
                Section("bss", 0x809ABD20, 0x809B35D0),
            ],
        ),
    },
}

DST_BINARIES = {
    "P": {
        "dol": DstBinary(0x80004000, 0x8038917C),
        "rel": DstBinary(0x80399180, 0x8076DB50),
    },
    "E": {
        "dol": DstBinary(0x80004000, 0x80384DFC),
        "rel": DstBinary(0x80394E00, 0x807693F0),
    },
    "J": {
        "dol": DstBinary(0x80004000, 0x80388AFC),
        "rel": DstBinary(0x80398B00, 0x8076CC90),
    },
    "K": {
        "dol": DstBinary(0x80004000, 0x8037719C),
        "rel": DstBinary(0x803871A0, 0x8075BFD0),
    },
}

CHUNKS = {
    "E": [
        Chunk(
            0x80004000, 0x80008030, 0x80004000
        ),  # This means that from 0x80004000 to 0x80008030 PAL is placed at 0x80004000 NTSC-U
        Chunk(0x8000808C, 0x8000ADBC, 0x8000804C),
        Chunk(0x8000AF24, 0x8000B6B4, 0x8000AE84),
        Chunk(0x8000B6B4, 0x8000C174, 0x80021048),
        Chunk(0x8000C174, 0x80021BA8, 0x8000B614),
        Chunk(0x80021BA8, 0x80225F1C, 0x80021B08),
        Chunk(0x802261F8, 0x802402E0, 0x80225E74),
        Chunk(0x802402E0, 0x80240E18, 0x802441FC),
        Chunk(0x80240E18, 0x80244DD4, 0x8023FF5C),
        Chunk(0x80276054, 0x8027B000, 0x8027A394),
        Chunk(0x802A4080, 0x80384C18, 0x8029FD00),
        Chunk(0x80384FD0, 0x80384FD4, 0x80380C50),
        Chunk(0x803858F0, 0x803858F4, 0x80381570),
        Chunk(0x80385908, 0x8038590C, 0x80381588),
        Chunk(0x80385FC0, 0x80386008, 0x80381C40),
        Chunk(0x803862C0, 0x803862C4, 0x80381F40),
        Chunk(0x80386338, 0x8038633C, 0x80381FB8),
        Chunk(0x80386D4C, 0x80386D50, 0x803829CC),
        Chunk(0x80386638, 0x80386644, 0x803822B8),
        Chunk(0x80386F48, 0x80386F90, 0x80382BC0),
        Chunk(0x805103B4, 0x80510A90, 0x8050C034),
        Chunk(0x80510B84, 0x8052D298, 0x8050C710),
        Chunk(0x8052D298, 0x8052D96C, 0x8054F800),
        Chunk(0x8052D96C, 0x8053D97C, 0x80528E24),
        Chunk(0x8053D97C, 0x8053E370, 0x8054FED4),
        Chunk(0x8053E370, 0x8054FB2C, 0x80538E34),
        Chunk(0x8054FB2C, 0x80550548, 0x805508C8),
        Chunk(0x80550548, 0x805537CC, 0x8054A5F0),
        Chunk(0x80553894, 0x8055572C, 0x8054D874),
        Chunk(0x8055572C, 0x8056AB6C, 0x805513AC),
        Chunk(0x8056AB6C, 0x8056B63C, 0x805AE0E8),
        Chunk(0x8056B63C, 0x80574030, 0x805667EC),
        Chunk(0x80574030, 0x805758AC, 0x805AEBB8),
        Chunk(0x80575A44, 0x80583F2C, 0x8056F1E0),
        Chunk(0x80583F2C, 0x8059B5A4, 0x8057D708),
        Chunk(0x8059B5A4, 0x8059EAA4, 0x805B058C),
        Chunk(0x8059EAA4, 0x805A0558, 0x80594D80),
        Chunk(0x805A068C, 0x805A1478, 0x805B3AC8),
        Chunk(0x805A1488, 0x805A1864, 0x805B48B4),
        Chunk(0x805A1864, 0x805A9EDC, 0x8059682C),
        Chunk(0x805A9EDC, 0x805AB574, 0x8059EEAC),
        Chunk(0x805AB574, 0x805ABC90, 0x805A0604),
        Chunk(0x805ABC90, 0x805ADD98, 0x805A0D28),
        Chunk(0x805ADD98, 0x805B9010, 0x805A2E70),
        Chunk(0x805B9010, 0x805B9300, 0x805B4C90),
        Chunk(0x805B9300, 0x805BAB40, 0x8061F7F4),
        Chunk(0x805BAB40, 0x805BB2B8, 0x805B4F80),
        Chunk(0x805BB2B8, 0x805BD2D8, 0x80621034),
        Chunk(0x805BD2D8, 0x805BD2E4, 0x805B695C),
        Chunk(0x805BD2E4, 0x805BD39C, 0x80623054),
        Chunk(0x805BD39C, 0x805BE600, 0x805B56F8),
        Chunk(0x805BE600, 0x805BE61C, 0x805B6968),
        Chunk(0x805BE66C, 0x805BE7F4, 0x8062310C),
        Chunk(0x805BE7F4, 0x805BE84C, 0x805B76D0),
        Chunk(0x805BE84C, 0x805BED68, 0x80623294),
        Chunk(0x805BED68, 0x805BED74, 0x805B7420),
        Chunk(0x805BED74, 0x805BF2D8, 0x806237B0),
        Chunk(0x805BF2D8, 0x805BF2DC, 0x805B919C),
        Chunk(0x805BF3CC, 0x805BFE1C, 0x805B69D0),
        Chunk(0x805BFE1C, 0x805C00C0, 0x805B742C),
        Chunk(0x805C00C0, 0x805C1B34, 0x805B7728),
        Chunk(0x805C1B34, 0x805C35F8, 0x805B91A0),
        Chunk(0x805C35F8, 0x805C3BCC, 0x80623E08),
        Chunk(0x805C3BCC, 0x805C4768, 0x805BAC64),
        Chunk(0x805C4768, 0x805C579C, 0x806243DC),
        Chunk(0x805C57B0, 0x805CADDC, 0x805BB800),
        Chunk(0x805CADDC, 0x805CC0A8, 0x80625424),
        Chunk(0x805CC1B4, 0x805CD94C, 0x80626758),
        Chunk(0x805CD94C, 0x805D1888, 0x805C0E2C),
        Chunk(0x805D1888, 0x805D1E84, 0x80627EF0),
        Chunk(0x805D1E84, 0x805D7F78, 0x805C4D68),
        Chunk(0x805D7F78, 0x805DBFDC, 0x806284EC),
        Chunk(0x805DC034, 0x805DE838, 0x8062C550),
        Chunk(0x805DE838, 0x805DE844, 0x805E4438),
        Chunk(0x805DE844, 0x805E0C38, 0x8062ED54),
        Chunk(0x805E0C38, 0x805E6B50, 0x805CAE5C),
        Chunk(0x805E6B50, 0x805E7460, 0x80631148),
        Chunk(0x805E7460, 0x805EA780, 0x805D0D74),
        Chunk(0x805EA780, 0x805EEB68, 0x80631A58),
        Chunk(0x805EEB68, 0x805F2D24, 0x805D4094),
        Chunk(0x805F2D24, 0x805F8B34, 0x80635E40),
        Chunk(0x805F8B34, 0x805FB5BC, 0x805D8250),
        Chunk(0x805FB5BC, 0x805FB820, 0x8063BC50),
        Chunk(0x805FB820, 0x805FD518, 0x805DACD8),
        Chunk(0x805FD518, 0x806012B0, 0x8063BEB4),
        Chunk(0x806012B0, 0x80608D18, 0x805DC9D0),
        Chunk(0x80608D18, 0x8060A72C, 0x805E4444),
        Chunk(0x8060A72C, 0x8061186C, 0x8063FC4C),
        Chunk(0x8061186C, 0x80616844, 0x805E5E58),
        Chunk(0x80616844, 0x8061AE6C, 0x80646D8C),
        Chunk(0x8061AE6C, 0x8061E898, 0x805EAE30),
        Chunk(0x8061E898, 0x8061F6E8, 0x8064B3B4),
        Chunk(0x8061F6E8, 0x80620CB4, 0x805EE85C),
        Chunk(0x80620D7C, 0x80634F48, 0x805EFEC8),
        Chunk(0x80634F48, 0x80637494, 0x806040B0),
        Chunk(0x80637494, 0x80637514, 0x806066A4),
        Chunk(0x80637514, 0x80637A30, 0x8064C204),
        Chunk(0x80637AC0, 0x8063BCF8, 0x80606720),
        Chunk(0x8063BE40, 0x806453E4, 0x8060AA20),
        Chunk(0x806453E4, 0x806467D4, 0x8064C78C),
        Chunk(0x806467D4, 0x8064A3F4, 0x80613FC4),
        Chunk(0x8064A3F4, 0x8064AEE4, 0x8064DB7C),
        Chunk(0x8064AEF8, 0x80651BEC, 0x80617BE4),
        Chunk(0x80651BEC, 0x80652300, 0x8064E66C),
        Chunk(0x80652300, 0x80653208, 0x8061E8EC),
        Chunk(0x80653208, 0x8065B354, 0x8064ED80),
        Chunk(0x8065B4E8, 0x8065C0EC, 0x80656ECC),
        Chunk(0x8065C0EC, 0x8065FA14, 0x8065CD74),
        Chunk(0x8065FA4C, 0x8065FE4C, 0x80657AD0),
        Chunk(0x8065FE4C, 0x80662778, 0x80657F10),
        Chunk(0x80662778, 0x80663194, 0x80660694),
        Chunk(0x80663194, 0x80665538, 0x8065A83C),
        Chunk(0x80665538, 0x80668814, 0x80673188),
        Chunk(0x80668814, 0x8067906C, 0x806610B0),
        Chunk(0x8067906C, 0x80679380, 0x80676464),
        Chunk(0x80679380, 0x8067AC00, 0x80671908),
        Chunk(0x8067AC00, 0x806BF4C8, 0x80676778),
        Chunk(0x806BF4C8, 0x806BFB14, 0x806E20A8),
        Chunk(0x806BFB14, 0x806C050C, 0x806BB040),
        Chunk(0x806C050C, 0x806C35A8, 0x806E26F4),
        Chunk(0x806C35A8, 0x806C3AA4, 0x806BBA38),
        Chunk(0x806C3AA4, 0x806C4ED4, 0x806E5790),
        Chunk(0x806C4EF0, 0x806C63A8, 0x806E6BC0),
        Chunk(0x806C63B0, 0x806C6B44, 0x806E8078),
        Chunk(0x806C6B44, 0x806CCE90, 0x806BBF50),
        Chunk(0x806CCE90, 0x806CE820, 0x806E880C),
        Chunk(0x806CE828, 0x806D02BC, 0x806C229C),
        Chunk(0x806D02BC, 0x806D28F4, 0x806EA19C),
        Chunk(0x806D2908, 0x806D5C5C, 0x806C3D38),
        Chunk(0x806D5C60, 0x806D5ED8, 0x806EC7D4),
        Chunk(0x806D5ED8, 0x806DA914, 0x806C7098),
        Chunk(0x806DA914, 0x806DB184, 0x806ECA58),
        Chunk(0x806DB184, 0x806DDA84, 0x806CBAD4),
        Chunk(0x806DDA84, 0x806DEB40, 0x806ED2C8),
        Chunk(0x806DEB40, 0x806DF7D0, 0x806CE3D4),
        Chunk(0x806DF7D0, 0x806DFD14, 0x806EE384),
        Chunk(0x806DFD14, 0x806E3A8C, 0x806CF064),
        Chunk(0x806E3A8C, 0x806E3E20, 0x806EE8C8),
        Chunk(0x806E3E20, 0x806E95B0, 0x806D2DDC),
        Chunk(0x806E95B0, 0x806EC7C0, 0x806EEC5C),
        Chunk(0x806EC7C0, 0x806ED53C, 0x806D8564),
        Chunk(0x806ED53C, 0x806F62FC, 0x806D92E8),
        Chunk(0x806F62FC, 0x806F7698, 0x806F1E74),
        Chunk(0x806F77C4, 0x806F7A54, 0x80713A78),
        Chunk(0x806F7AA8, 0x806F8220, 0x80713D08),
        Chunk(0x806F8220, 0x806F8934, 0x806F3210),
        Chunk(0x806F8934, 0x806FA3FC, 0x806F3978),
        Chunk(0x806FA3FC, 0x806FE240, 0x806F5494),
        Chunk(0x806FE240, 0x806FE9E0, 0x806F9330),
        Chunk(0x806FE9E0, 0x807001A4, 0x80714480),
        Chunk(0x80700230, 0x80700474, 0x80715C44),
        Chunk(0x80700474, 0x8070E7B8, 0x806F9AD0),
        Chunk(0x8070E7B8, 0x8070F8B8, 0x80715E88),
        Chunk(0x8070F8B8, 0x807179C4, 0x80707E14),
        Chunk(0x807179C4, 0x80717E34, 0x8070FFAC),
        Chunk(0x80717E34, 0x807182E8, 0x80716F88),
        Chunk(0x807182E8, 0x8071B86C, 0x8071041C),
        Chunk(0x8071B86C, 0x80726574, 0x8071743C),
        Chunk(0x80726574, 0x8072820C, 0x8073C5DC),
        Chunk(0x8072821C, 0x807285C8, 0x8073E274),
        Chunk(0x807285CC, 0x80729338, 0x8073E620),
        Chunk(0x80729350, 0x80729B88, 0x80722144),
        Chunk(0x80729B88, 0x8072A894, 0x80722984),
        Chunk(0x8072A894, 0x8072B95C, 0x80723698),
        Chunk(0x8072B95C, 0x8072DE64, 0x80724770),
        Chunk(0x8072DFCC, 0x8072FF60, 0x8073F390),
        Chunk(0x8072FF78, 0x80730198, 0x80741324),
        Chunk(0x80730198, 0x80730A80, 0x80726DE8),
        Chunk(0x80730B40, 0x80731960, 0x80741544),
        Chunk(0x80731960, 0x80735948, 0x807277A8),
        Chunk(0x80735948, 0x80738DB8, 0x80742364),
        Chunk(0x80738DB8, 0x8073C54C, 0x8072B790),
        Chunk(0x8073C54C, 0x8073EDF0, 0x807457D4),
        Chunk(0x8073EDF0, 0x8074C4A8, 0x8072EF24),
        Chunk(0x8074C4A8, 0x8074D5B8, 0x8076CEAC),
        Chunk(0x8074D5B8, 0x807519C8, 0x80748078),
        Chunk(0x807519C8, 0x80754104, 0x8076DFBC),
        Chunk(0x80754104, 0x80758BDC, 0x8074C488),
        Chunk(0x80758BDC, 0x8075DB24, 0x807706F8),
        Chunk(0x8075DB3C, 0x8075E78C, 0x80750F60),
        Chunk(0x8075E78C, 0x8075EAFC, 0x80775658),
        Chunk(0x8075EAFC, 0x80765C94, 0x80751BB0),
        Chunk(0x80765C94, 0x807678F4, 0x807759C8),
        Chunk(0x807678F4, 0x80768D20, 0x80758D48),
        Chunk(0x80768D20, 0x807693EC, 0x80777628),
        Chunk(0x8076960C, 0x8076C85C, 0x80777E68),
        Chunk(0x8076C85C, 0x8076EBDC, 0x8075A174),
        Chunk(0x8076EBE0, 0x8076F2DC, 0x8077B0B8),
        Chunk(0x8076F2DC, 0x807726C4, 0x8075C4F4),
        Chunk(0x80772704, 0x80773C14, 0x8077B7B8),
        Chunk(0x80773C1C, 0x8077439C, 0x8075F8DC),
        Chunk(0x807743A0, 0x807787F0, 0x8076010C),
        Chunk(0x807787F0, 0x8077902C, 0x8077CD10),
        Chunk(0x8077902C, 0x8077CE88, 0x8076455C),
        Chunk(0x8077CEC8, 0x8077DF24, 0x8077D54C),
        Chunk(0x8077DF24, 0x807829D8, 0x807683F8),
        Chunk(0x807829D8, 0x80787D84, 0x8077E5A8),
        Chunk(0x80787D84, 0x8078C960, 0x807D031C),
        Chunk(0x8078C960, 0x807A81B4, 0x80783954),
        Chunk(0x807A81B4, 0x807A9D70, 0x807D4F74),
        Chunk(0x807A9EB8, 0x807AF140, 0x8079F210),
        Chunk(0x807AF140, 0x807B2EF8, 0x807D6B94),
        Chunk(0x807B2EF8, 0x807D976C, 0x807A4498),
        Chunk(0x807D976C, 0x807D9B80, 0x807DA94C),
        Chunk(0x807D9B98, 0x807DA5C0, 0x807CAD0C),
        Chunk(0x807DA5C0, 0x807DBCCC, 0x807DAD78),
        Chunk(0x807DBCCC, 0x807DC8C8, 0x807CB734),
        Chunk(0x807DC950, 0x807E093C, 0x807CC330),
        Chunk(0x807E093C, 0x807E2520, 0x8083C6F0),
        Chunk(0x807E259C, 0x807E5610, 0x807DC50C),
        Chunk(0x807E5654, 0x807E6414, 0x807DF5E4),
        Chunk(0x807E6414, 0x807E9C44, 0x8083E33C),
        Chunk(0x807E9C50, 0x807EDD98, 0x807E03A4),
        Chunk(0x807EDD98, 0x807EE23C, 0x80841B6C),
        Chunk(0x807EE250, 0x807EE468, 0x807E44EC),
        Chunk(0x807EE474, 0x807EEA14, 0x80842010),
        Chunk(0x807EEA14, 0x807EF9F4, 0x807E4704),
        Chunk(0x807EFD0C, 0x807F76EC, 0x807E56E4),
        Chunk(0x807F76EC, 0x807F7BA4, 0x808428E8),
        Chunk(0x807F7BC4, 0x807F890C, 0x807ED0C4),
        Chunk(0x807F8968, 0x807F9280, 0x80842DA0),
        Chunk(0x807F9580, 0x807FAB58, 0x808438A0),
        Chunk(0x807FAB58, 0x807FEB68, 0x807EDF98),
        Chunk(0x807FEB68, 0x807FFAE0, 0x80844E78),
        Chunk(0x807FFB20, 0x80805A0C, 0x807F1FA8),
        Chunk(0x80805A0C, 0x8080761C, 0x80845DF0),
        Chunk(0x8080761C, 0x80809448, 0x807F7ED4),
        Chunk(0x80809448, 0x8080AD20, 0x80847A00),
        Chunk(0x8080AD20, 0x80811E48, 0x807F9D00),
        Chunk(0x80811E48, 0x80813BD4, 0x808492D8),
        Chunk(0x80813BD4, 0x8081E284, 0x80800E28),
        Chunk(0x8081E284, 0x8081EFEC, 0x8084B064),
        Chunk(0x8081EFEC, 0x8082E540, 0x8080B4D8),
        Chunk(0x8082E540, 0x8082E854, 0x8084BDCC),
        Chunk(0x8082E854, 0x8082F408, 0x8081AA2C),
        Chunk(0x8082F408, 0x808334A0, 0x8084C0E0),
        Chunk(0x808334E0, 0x80833B00, 0x8081B5E0),
        Chunk(0x80833B00, 0x80838E4C, 0x8081BC08),
        Chunk(0x80838E60, 0x8083B0C0, 0x808501B8),
        Chunk(0x8083B0CC, 0x8083CB44, 0x80820F54),
        Chunk(0x8083CB44, 0x8083D42C, 0x80852424),
        Chunk(0x8083D42C, 0x80842334, 0x808229CC),
        Chunk(0x80842340, 0x808447AC, 0x80852D0C),
        Chunk(0x808447AC, 0x8084A9A0, 0x808278D4),
        Chunk(0x8084A9A0, 0x8084D0DC, 0x80855184),
        Chunk(0x8084D0DC, 0x80851D2C, 0x8082DAC8),
        Chunk(0x80851D38, 0x80852C60, 0x808578C0),
        Chunk(0x80852C60, 0x80853CA4, 0x80832718),
        Chunk(0x80853CA4, 0x808551EC, 0x808587F4),
        Chunk(0x808551EC, 0x8085C3CC, 0x8083375C),
        Chunk(0x8085C3CC, 0x8085E674, 0x80859D3C),
        Chunk(0x8085E674, 0x8085F0AC, 0x8083A950),
        Chunk(0x8085F0AC, 0x8085FFD4, 0x8085BFE4),
        Chunk(0x8085FFD4, 0x80860F2C, 0x8083B388),
        Chunk(0x80860F2C, 0x80862E24, 0x8085CF0C),
        Chunk(0x80863234, 0x8086708C, 0x8085EE04),
        Chunk(0x8086708C, 0x808676E0, 0x80864D38),
        Chunk(0x808676E0, 0x808697BC, 0x80862C5C),
        Chunk(0x808697BC, 0x8086A254, 0x8086538C),
        Chunk(0x8086A254, 0x8086C098, 0x808766F4),
        Chunk(0x8086C108, 0x8086C988, 0x80878538),
        Chunk(0x8086CA40, 0x80872CA4, 0x80878DB8),
        Chunk(0x80872CA4, 0x808739B0, 0x80865E24),
        Chunk(0x808739B0, 0x80875454, 0x80866BE8),
        Chunk(0x80875454, 0x8088344C, 0x808686FC),
        Chunk(0x8088344C, 0x8088F400, 0x8087F01C),
        Chunk(0x80891B28, 0x80891B2C, 0x8088D610),
        Chunk(0x808A10D8, 0x808A10DC, 0x8089C9D0),
        Chunk(0x808B3984, 0x808B3988, 0x808AF134),
        Chunk(0x808B5B1C, 0x808B5B20, 0x808B125C),
        Chunk(0x808B5C78, 0x808B5C7C, 0x808B13B8),
        Chunk(0x808CB550, 0x808CB554, 0x808C6048),
        Chunk(0x808D3698, 0x808D369C, 0x808D5148),
        Chunk(0x808D36CC, 0x808D36D0, 0x808D517C),
        Chunk(0x808D36D4, 0x808D36D8, 0x808D5184),
        Chunk(0x808D3744, 0x808D3748, 0x808D51F4),
        Chunk(0x808D374C, 0x808D3750, 0x808D51FC),
        Chunk(0x808D3E14, 0x808D3E18, 0x808CEEAC),
        Chunk(0x808DA318, 0x808DA368, 0x808D3F60),
        Chunk(0x809BD6E8, 0x809BD6EC, 0x809B8F28),
        Chunk(0x809BD70C, 0x809BD710, 0x809B8F4C),
        Chunk(0x809BD728, 0x809BD72C, 0x809B8F68),
        Chunk(0x809BD730, 0x809BD734, 0x809B8F70),
        Chunk(0x809BD738, 0x809BD73C, 0x809B8F78),
        Chunk(0x809BD740, 0x809BD744, 0x809B8F80),
        Chunk(0x809BD748, 0x809BD74C, 0x809B8F88),
        Chunk(0x809C1830, 0x809C1834, 0x809BD070),
        Chunk(0x809C18F8, 0x809C18FC, 0x809BD110),
        Chunk(0x809C1988, 0x809C198C, 0x809BD378),
        Chunk(0x809C1E38, 0x809C1E3C, 0x809BD508),
        Chunk(0x809C21D0, 0x809C21D4, 0x809BDA10),
        Chunk(0x809C2328, 0x809C232C, 0x809BDB60),
        Chunk(0x809C27F0, 0x809C27FC, 0x809BDBB0),
        Chunk(0x809C2850, 0x809C2854, 0x809BDC10),
        Chunk(0x809C2F38, 0x809C2F3C, 0x809BE740),
        Chunk(0x809C38B8, 0x809C38BC, 0x809BF0B0),
        Chunk(0x809C4680, 0x809C4684, 0x809BFDC0),
        Chunk(0x809C4740, 0x809C4744, 0x809BFF90),
    ],
    "J": [
        Chunk(0x80004000, 0x80008024, 0x80004000),
        Chunk(0x800080E8, 0x8000ADC0, 0x80008044),
        Chunk(0x8000AF08, 0x80021BAC, 0x8000AE2C),
        Chunk(0x80021BAC, 0x80244DE0, 0x80021ACC),
        Chunk(0x802A4080, 0x8038917C, 0x802A3A00),
        Chunk(0x80276054, 0x8027B000, 0x802759F4),
        Chunk(0x80384FD0, 0x80384FD4, 0x80384950),
        Chunk(0x803858F0, 0x803858F4, 0x80385270),
        Chunk(0x805103B4, 0x805CC0A8, 0x8050FD34),
        Chunk(0x805CC1B4, 0x805FA33C, 0x805CBA90),
        Chunk(0x805FA344, 0x805FF6E8, 0x805F9C20),
        Chunk(0x805FFD70, 0x806003E8, 0x805FF528),
        Chunk(0x80600C78, 0x80620CB4, 0x806003EC),
        Chunk(0x80620D7C, 0x80637A24, 0x806204C8),
        Chunk(0x80637A80, 0x8063BCF8, 0x8063716C),
        Chunk(0x8063BE40, 0x8088F400, 0x8063B4AC),
        Chunk(0x80891B28, 0x80891B2C, 0x80891178),
        Chunk(0x80899692, 0x808B5CBF, 0x808987F2),
        Chunk(0x808B5B1C, 0x808B5B20, 0x808B4C7C),
        Chunk(0x808B5C78, 0x808B5C7C, 0x808B4DD8),
        Chunk(0x808CB550, 0x808CB554, 0x808CA6A0),
        Chunk(0x808D3698, 0x808D369C, 0x808D27E8),
        Chunk(0x808D36CC, 0x808D36D0, 0x808D281C),
        Chunk(0x808D36D4, 0x808D36D8, 0x808D2824),
        Chunk(0x808D3744, 0x808D3748, 0x808D2894),
        Chunk(0x808D374C, 0x808D3750, 0x808D289C),
        Chunk(0x808D3E14, 0x808D3E18, 0x808D2F64),
        Chunk(0x808DA318, 0x808DA368, 0x808D9468),
        Chunk(0x809BD6E8, 0x809BD6EC, 0x809BC748),
        Chunk(0x809BD70C, 0x809BD710, 0x809BC76C),
        Chunk(0x809BD728, 0x809BD72C, 0x809BC788),
        Chunk(0x809BD730, 0x809BD734, 0x809BC790),
        Chunk(0x809BD738, 0x809BD73C, 0x809BC798),
        Chunk(0x809BD740, 0x809BD744, 0x809BC7A0),
        Chunk(0x809BD748, 0x809BD74C, 0x809BC7A8),
        Chunk(0x809C1830, 0x809C1834, 0x809C0890),
        Chunk(0x809C18F8, 0x809C18FC, 0x809C0958),
        Chunk(0x809C1988, 0x809C198C, 0x809C09E8),
        Chunk(0x809C1E38, 0x809C1E3C, 0x809C0E98),
        Chunk(0x809C21D0, 0x809C21D4, 0x809C1230),
        Chunk(0x809C2328, 0x809C232C, 0x809C1388),
        Chunk(0x809C27F0, 0x809C27FC, 0x809C1850),
        Chunk(0x809C2850, 0x809C2854, 0x809C18B0),
        Chunk(0x809C2F38, 0x809C2F3C, 0x809C1F98),
        Chunk(0x809C38B8, 0x809C38BC, 0x809C2918),
        Chunk(0x809C4680, 0x809C4684, 0x809C36E0),
        Chunk(0x809C4740, 0x809C4744, 0x809C37A0),
    ],
    "K": [
        Chunk(0x80004000, 0x800074DC, 0x80004000),
        Chunk(0x800077C8, 0x800079D4, 0x80007894),
        Chunk(0x80007BC0, 0x80007BCC, 0x80007CAC),
        Chunk(0x80007F2C, 0x80008004, 0x80008034),
        Chunk(0x8000829C, 0x80008BA4, 0x8000841C),
        Chunk(0x80008C04, 0x800093FC, 0x80008D54),
        Chunk(0x80009458, 0x8000AD14, 0x80009560),
        Chunk(0x8000AF24, 0x8000B610, 0x8000AFD0),
        Chunk(0x8000B654, 0x80021BA8, 0x8000B6BC),
        Chunk(0x80021BB0, 0x800EA448, 0x80021C10),
        Chunk(0x800EA474, 0x801642F4, 0x800EA4EC),
        Chunk(0x80164310, 0x801746FC, 0x801643AC),
        Chunk(0x801746FC, 0x80174C54, 0x80174838),
        Chunk(0x80174EF4, 0x80175970, 0x8017517C),
        Chunk(0x80175978, 0x80176B58, 0x80175BF0),
        Chunk(0x80176D68, 0x801774D0, 0x80176FF8),
        Chunk(0x80178514, 0x80178E8C, 0x801788A4),
        Chunk(0x8017A0BC, 0x8017AC74, 0x8017A3AC),
        Chunk(0x8017B338, 0x8017B73C, 0x8017B790),
        Chunk(0x8017B740, 0x8017DC3C, 0x8017BB98),
        Chunk(0x8017E650, 0x8017EBC4, 0x8017EAA8),
        Chunk(0x8017F674, 0x801E8414, 0x8017F9D0),
        Chunk(0x801E8414, 0x8020FD10, 0x801E883C),
        Chunk(0x8020FD18, 0x8020FD8C, 0x80210138),
        Chunk(0x8020FE24, 0x8021008C, 0x802101AC),
        Chunk(0x802100A0, 0x80244DE0, 0x80210414),
        Chunk(0x80276054, 0x8027B000, 0x80263F04),
        Chunk(0x802A4080, 0x803858E0, 0x80292080),
        Chunk(0x80384FD0, 0x80384FD4, 0x80372FD0),
        Chunk(0x803858F0, 0x803858F4, 0x803738F8),
        Chunk(0x80385908, 0x8038590C, 0x80373910),
        Chunk(0x80385FC0, 0x8038917C, 0x80373FE0),
        Chunk(0x805103B4, 0x8051D72C, 0x804FE3D4),
        Chunk(0x8051E488, 0x8052A324, 0x8050C4AC),
        Chunk(0x8052A338, 0x805C08CC, 0x80518390),
        Chunk(0x805C08D4, 0x805CC0A8, 0x805AE938),
        Chunk(0x805CC220, 0x805CEAE0, 0x805BA1E0),
        Chunk(0x805CEAFC, 0x805CF0E8, 0x805BCABC),
        Chunk(0x805CF154, 0x805CF158, 0x805BD118),
        Chunk(0x805CF2BC, 0x805CF2C0, 0x805BD284),
        Chunk(0x805CF7E4, 0x805CF7E8, 0x805BD738),
        Chunk(0x805CF8BC, 0x805D00D0, 0x805BDA40),
        Chunk(0x805D01C8, 0x805D124C, 0x805BE350),
        Chunk(0x805D1260, 0x805EEB68, 0x805BF3FC),
        Chunk(0x805EEB68, 0x805FA33C, 0x805DCF88),
        Chunk(0x805FA344, 0x80620CB4, 0x805E8764),
        Chunk(0x80620D7C, 0x80637A24, 0x8060F174),
        Chunk(0x80637A80, 0x8063BCF8, 0x80625E18),
        Chunk(0x8063BE40, 0x806681E8, 0x8062A158),
        Chunk(0x80668334, 0x80675464, 0x8065668C),
        Chunk(0x80675808, 0x80675EB8, 0x80663B68),
        Chunk(0x80675F2C, 0x806771F8, 0x8066428C),
        Chunk(0x80677C3C, 0x80678134, 0x80665FE4),
        Chunk(0x8067818C, 0x80742B58, 0x80666534),
        Chunk(0x80743154, 0x8088F400, 0x80731514),
        Chunk(0x80891B28, 0x80891B2C, 0x8087FF30),
        Chunk(0x808A10D8, 0x808A10DC, 0x8088F538),
        Chunk(0x808B3984, 0x808B3988, 0x808A1DFC),
        Chunk(0x808B5B1C, 0x808B5B20, 0x808A3F94),
        Chunk(0x808B5C78, 0x808B5C7C, 0x808A40F0),
        Chunk(0x808CB550, 0x808CB554, 0x808B99E8),
        Chunk(0x808D3698, 0x808D369C, 0x808C1B30),
        Chunk(0x808D36CC, 0x808D36D0, 0x808C1B64),
        Chunk(0x808D36D4, 0x808D36D8, 0x808C1B6C),
        Chunk(0x808D3744, 0x808D3748, 0x808C1BDC),
        Chunk(0x808D374C, 0x808D3750, 0x808C1BE4),
        Chunk(0x808D3E14, 0x808D3E18, 0x808C22AC),
        Chunk(0x808DA318, 0x808DA368, 0x808C87B0),
        Chunk(0x809BD6E8, 0x809BD6EC, 0x809ABD28),
        Chunk(0x809BD70C, 0x809BD710, 0x809ABD4C),
        Chunk(0x809BD728, 0x809BD72C, 0x809ABD68),
        Chunk(0x809BD730, 0x809BD734, 0x809ABD70),
        Chunk(0x809BD738, 0x809BD73C, 0x809ABD78),
        Chunk(0x809BD740, 0x809BD744, 0x809ABD80),
        Chunk(0x809BD748, 0x809BD74C, 0x809ABD88),
        Chunk(0x809C1830, 0x809C1834, 0x809AFE70),
        Chunk(0x809C18F8, 0x809C18FC, 0x809AFF38),
        Chunk(0x809C1988, 0x809C198C, 0x809AFFC8),
        Chunk(0x809C1E38, 0x809C1E3C, 0x809B0478),
        Chunk(0x809C21D0, 0x809C21D4, 0x809B0810),
        Chunk(0x809C2328, 0x809C232C, 0x809B0968),
        Chunk(0x809C27F0, 0x809C27FC, 0x809B0E30),
        Chunk(0x809C2850, 0x809C2854, 0x809B0E90),
        Chunk(0x809C2F38, 0x809C2F3C, 0x809B1578),
        Chunk(0x809C38B8, 0x809C38BC, 0x809B13F8),
        Chunk(0x809C4680, 0x809C4684, 0x809B2CC0),
        Chunk(0x809C4740, 0x809C4744, 0x809B2D80),
    ],
}


def write_symbol(out_file, name, address):
    # address = address.replace('0x', '')
    out_file.write(f"{address:#x}:{name}\n")


def get_binary_name(region, address):
    return next(
        module
        for module, sections in SRC_BINARIES[region].items()
        if address in sections
    )


def port(region, address):
    if region == "P":
        return address

    return next(
        (chunk.port(address) for chunk in CHUNKS[region] if address in chunk), None
    )


parser = ArgumentParser()
parser.add_argument("region")
parser.add_argument("in_path")
parser.add_argument("out_path")
parser.add_argument("--base", action="store_true")
args = parser.parse_args()

validRegionList = ["P", "E", "J", "K"]
if args.region not in validRegionList:
    sys.exit(
        f'The specified region \'{args.region}\' is invalid! Valid regions include: {", ".join(validRegionList)}!'
    )

with open(args.out_path, "w") as out_file:
    if args.base:
        for region in DST_BINARIES:
            for module in DST_BINARIES[region]:
                DST_BINARIES[region][module].start = SRC_BINARIES[region][module].start
                DST_BINARIES[region][module].end = max(
                    section.end for section in SRC_BINARIES[region][module].sections
                )

    # Write the start and end address for each section in the rel
    for i in range(len(SRC_BINARIES[args.region]["rel"].sections)):
        section = SRC_BINARIES[args.region]["rel"].sections[i]

    with open(args.in_path, "r") as symbols:
        for symbol in symbols.readlines():
            if symbol.isspace():
                out_file.write("\n")
                continue
            address, name = symbol.split()
            address = int(address, 16)

            # At the moment, this script only supports porting addresses from the PAL version of the game to other versions of the game
            binary_name = get_binary_name("P", address)
            bss_section = next(
                (
                    section
                    for section in SRC_BINARIES["P"]["rel"].sections
                    if section.name == "bss"
                ),
                None,
            )
            if bss_section is None:
                sys.exit(
                    "Couldn't find the '.bss' section of the 'StaticR.rel' module!"
                )
            is_rel_bss = bss_section.start <= address < bss_section.end

            address = port(args.region, address)
            if address is None:
                sys.exit(f"Couldn't port symbol {name} to region {args.region}!")
            write_symbol(out_file, name, address)
    # Write in constant variables that are not within dol range
    # Per-race variables
    write_symbol(out_file, "HideNSeekData", 0x800001B0)
    write_symbol(out_file, "SpectatorMode", 0x800001E4)
    write_symbol(out_file, "CurrentSpectated", 0x800001E5)
    write_symbol(out_file, "AmIDead", 0x800001E6)
    write_symbol(out_file, "Have30SecondsPassed", 0x800001E7)
    write_symbol(out_file, "UseReplayCams", 0x800001E8)
    write_symbol(out_file, "MusicState", 0x800001E9)
    write_symbol(out_file, "ENPTCRC", 0x800001EC)
    # Per-GP variables
    write_symbol(out_file, "IsInfection", 0x800002B0)
    write_symbol(out_file, "SeekerCount", 0x800002B1)
    write_symbol(out_file, "AlwaysWinVote", 0x800002B2)
    write_symbol(out_file, "HalfTimer", 0x800002B3)
    write_symbol(out_file, "GlitchesAllowed", 0x800002B5)
    write_symbol(out_file, "BtGlitch", 0x800002B6)
    write_symbol(out_file, "NoIncibility", 0x800002B7)
    write_symbol(out_file, "PrevSeekers", 0x800002B8)
    # Other HnS variables
    write_symbol(out_file, "Version", 0x800003B0)
    write_symbol(out_file, "KillData", 0x800004B0)
    write_symbol(out_file, "NoMusic", 0x80004000)
    write_symbol(out_file, "ThirtyFPS", 0x8000400F)
    out_file.write("\n")


with open(args.out_path, "r") as symbols:
    filedata = symbols.read()
    filedata = filedata.replace("0x", "")

with open(args.out_path, "w") as symbols:
    symbols.write(filedata)
