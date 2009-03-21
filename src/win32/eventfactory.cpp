#include <iostream>
#include <map>

#include <boost/cstdint.hpp>

#include <wm/exception.hpp>
#include <wm/event.hpp>
#include <wm/events.hpp>
#include "impl/eventfactory.hpp"
#include "impl/keymap.hpp"

#include <windows.h>

namespace
{
	const char *message_names[] = {
		"WM_NULL",
		"WM_CREATE",
		"WM_DESTROY",
		"WM_MOVE",
		"UNKNOWN (4)",
		"WM_SIZE",
		"WM_ACTIVATE",
		"WM_SETFOCUS",
		"WM_KILLFOCUS",
		"UNKNOWN (9)",
		"WM_ENABLE",
		"WM_SETREDRAW",
		"WM_SETTEXT",
		"WM_GETTEXT",
		"WM_GETTEXTLENGTH",
		"WM_PAINT",
		"WM_CLOSE",
		"WM_QUERYENDSESSION",
		"WM_QUIT",
		"WM_QUERYOPEN",
		"WM_ERASEBKGND",
		"WM_SYSCOLORCHANGE",
		"WM_ENDSESSION",
		"UNKNOWN (23)",
		"WM_SHOWWINDOW",
		"UNKNOWN (25)",
		"WM_SETTINGCHANGE",
		"WM_DEVMODECHANGE",
		"WM_ACTIVATEAPP",
		"WM_FONTCHANGE",
		"WM_TIMECHANGE",
		"WM_CANCELMODE",
		"WM_SETCURSOR",
		"WM_MOUSEACTIVATE",
		"WM_CHILDACTIVATE",
		"WM_QUEUESYNC",
		"WM_GETMINMAXINFO",
		"UNKNOWN (37)",
		"WM_PAINTICON",
		"WM_ICONERASEBKGND",
		"WM_NEXTDLGCTL",
		"UNKNOWN (41)",
		"WM_SPOOLERSTATUS",
		"WM_DRAWITEM",
		"WM_MEASUREITEM",
		"WM_DELETEITEM",
		"WM_VKEYTOITEM",
		"WM_CHARTOITEM",
		"WM_SETFONT",
		"WM_GETFONT",
		"WM_SETHOTKEY",
		"WM_GETHOTKEY",
		"UNKNOWN (52)",
		"UNKNOWN (53)",
		"UNKNOWN (54)",
		"WM_QUERYDRAGICON",
		"UNKNOWN (56)",
		"WM_COMPAREITEM",
		"UNKNOWN (58)",
		"UNKNOWN (59)",
		"UNKNOWN (60)",
		"WM_GETOBJECT",
		"UNKNOWN (62)",
		"UNKNOWN (63)",
		"UNKNOWN (64)",
		"WM_COMPACTING",
		"UNKNOWN (66)",
		"UNKNOWN (67)",
		"WM_COMMNOTIFY",
		"UNKNOWN (69)",
		"WM_WINDOWPOSCHANGING",
		"WM_WINDOWPOSCHANGED",
		"WM_POWER",
		"UNKNOWN (73)",
		"WM_COPYDATA",
		"WM_CANCELJOURNAL",
		"UNKNOWN (76)",
		"UNKNOWN (77)",
		"WM_NOTIFY",
		"UNKNOWN (79)",
		"WM_INPUTLANGCHANGEREQUEST",
		"WM_INPUTLANGCHANGE",
		"WM_TCARD",
		"WM_HELP",
		"WM_USERCHANGED",
		"WM_NOTIFYFORMAT",
		"UNKNOWN (86)",
		"UNKNOWN (87)",
		"UNKNOWN (88)",
		"UNKNOWN (89)",
		"UNKNOWN (90)",
		"UNKNOWN (91)",
		"UNKNOWN (92)",
		"UNKNOWN (93)",
		"UNKNOWN (94)",
		"UNKNOWN (95)",
		"UNKNOWN (96)",
		"UNKNOWN (97)",
		"UNKNOWN (98)",
		"UNKNOWN (99)",
		"UNKNOWN (100)",
		"UNKNOWN (101)",
		"UNKNOWN (102)",
		"UNKNOWN (103)",
		"UNKNOWN (104)",
		"UNKNOWN (105)",
		"UNKNOWN (106)",
		"UNKNOWN (107)",
		"UNKNOWN (108)",
		"UNKNOWN (109)",
		"UNKNOWN (110)",
		"UNKNOWN (111)",
		"UNKNOWN (112)",
		"UNKNOWN (113)",
		"UNKNOWN (114)",
		"UNKNOWN (115)",
		"UNKNOWN (116)",
		"UNKNOWN (117)",
		"UNKNOWN (118)",
		"UNKNOWN (119)",
		"UNKNOWN (120)",
		"UNKNOWN (121)",
		"UNKNOWN (122)",
		"WM_CONTEXTMENU",
		"WM_STYLECHANGING",
		"WM_STYLECHANGED",
		"WM_DISPLAYCHANGE",
		"WM_GETICON",
		"WM_SETICON",
		"WM_NCCREATE",
		"WM_NCDESTROY",
		"WM_NCCALCSIZE",
		"WM_NCHITTEST",
		"WM_NCPAINT",
		"WM_NCACTIVATE",
		"WM_GETDLGCODE",
		"WM_SYNCPAINT",
		"UNKNOWN (137)",
		"UNKNOWN (138)",
		"UNKNOWN (139)",
		"UNKNOWN (140)",
		"UNKNOWN (141)",
		"UNKNOWN (142)",
		"UNKNOWN (143)",
		"UNKNOWN (144)",
		"UNKNOWN (145)",
		"UNKNOWN (146)",
		"UNKNOWN (147)",
		"UNKNOWN (148)",
		"UNKNOWN (149)",
		"UNKNOWN (150)",
		"UNKNOWN (151)",
		"UNKNOWN (152)",
		"UNKNOWN (153)",
		"UNKNOWN (154)",
		"UNKNOWN (155)",
		"UNKNOWN (156)",
		"UNKNOWN (157)",
		"UNKNOWN (158)",
		"UNKNOWN (159)",
		"WM_NCMOUSEMOVE",
		"WM_NCLBUTTONDOWN",
		"WM_NCLBUTTONUP",
		"WM_NCLBUTTONDBLCLK",
		"WM_NCRBUTTONDOWN",
		"WM_NCRBUTTONUP",
		"WM_NCRBUTTONDBLCLK",
		"WM_NCMBUTTONDOWN",
		"WM_NCMBUTTONUP",
		"WM_NCMBUTTONDBLCLK",
		"UNKNOWN (170)",
		"UNKNOWN (171)",
		"UNKNOWN (172)",
		"UNKNOWN (173)",
		"UNKNOWN (174)",
		"UNKNOWN (175)",
		"UNKNOWN (176)",
		"UNKNOWN (177)",
		"UNKNOWN (178)",
		"UNKNOWN (179)",
		"UNKNOWN (180)",
		"UNKNOWN (181)",
		"UNKNOWN (182)",
		"UNKNOWN (183)",
		"UNKNOWN (184)",
		"UNKNOWN (185)",
		"UNKNOWN (186)",
		"UNKNOWN (187)",
		"UNKNOWN (188)",
		"UNKNOWN (189)",
		"UNKNOWN (190)",
		"UNKNOWN (191)",
		"UNKNOWN (192)",
		"UNKNOWN (193)",
		"UNKNOWN (194)",
		"UNKNOWN (195)",
		"UNKNOWN (196)",
		"UNKNOWN (197)",
		"UNKNOWN (198)",
		"UNKNOWN (199)",
		"UNKNOWN (200)",
		"UNKNOWN (201)",
		"UNKNOWN (202)",
		"UNKNOWN (203)",
		"UNKNOWN (204)",
		"UNKNOWN (205)",
		"UNKNOWN (206)",
		"UNKNOWN (207)",
		"UNKNOWN (208)",
		"UNKNOWN (209)",
		"UNKNOWN (210)",
		"UNKNOWN (211)",
		"UNKNOWN (212)",
		"UNKNOWN (213)",
		"UNKNOWN (214)",
		"UNKNOWN (215)",
		"UNKNOWN (216)",
		"UNKNOWN (217)",
		"UNKNOWN (218)",
		"UNKNOWN (219)",
		"UNKNOWN (220)",
		"UNKNOWN (221)",
		"UNKNOWN (222)",
		"UNKNOWN (223)",
		"UNKNOWN (224)",
		"UNKNOWN (225)",
		"UNKNOWN (226)",
		"UNKNOWN (227)",
		"UNKNOWN (228)",
		"UNKNOWN (229)",
		"UNKNOWN (230)",
		"UNKNOWN (231)",
		"UNKNOWN (232)",
		"UNKNOWN (233)",
		"UNKNOWN (234)",
		"UNKNOWN (235)",
		"UNKNOWN (236)",
		"UNKNOWN (237)",
		"UNKNOWN (238)",
		"UNKNOWN (239)",
		"UNKNOWN (240)",
		"UNKNOWN (241)",
		"UNKNOWN (242)",
		"UNKNOWN (243)",
		"UNKNOWN (244)",
		"UNKNOWN (245)",
		"UNKNOWN (246)",
		"UNKNOWN (247)",
		"UNKNOWN (248)",
		"UNKNOWN (249)",
		"UNKNOWN (250)",
		"UNKNOWN (251)",
		"UNKNOWN (252)",
		"UNKNOWN (253)",
		"UNKNOWN (254)",
		"UNKNOWN (255)",
		"WM_KEYDOWN",
		"WM_KEYUP",
		"WM_CHAR",
		"WM_DEADCHAR",
		"WM_SYSKEYDOWN",
		"WM_SYSKEYUP",
		"WM_SYSCHAR",
		"WM_SYSDEADCHAR",
		"WM_KEYLAST",
		"UNKNOWN (265)",
		"UNKNOWN (266)",
		"UNKNOWN (267)",
		"UNKNOWN (268)",
		"UNKNOWN (269)",
		"UNKNOWN (270)",
		"UNKNOWN (271)",
		"WM_INITDIALOG",
		"WM_COMMAND",
		"WM_SYSCOMMAND",
		"WM_TIMER",
		"WM_HSCROLL",
		"WM_VSCROLL",
		"WM_INITMENU",
		"WM_INITMENUPOPUP",
		"UNKNOWN (280)",
		"UNKNOWN (281)",
		"UNKNOWN (282)",
		"UNKNOWN (283)",
		"UNKNOWN (284)",
		"UNKNOWN (285)",
		"UNKNOWN (286)",
		"WM_MENUSELECT",
		"WM_MENUCHAR",
		"WM_ENTERIDLE",
		"WM_MENURBUTTONUP",
		"WM_MENUDRAG",
		"WM_MENUGETOBJECT",
		"WM_UNINITMENUPOPUP",
		"WM_MENUCOMMAND",
		"UNKNOWN (295)",
		"UNKNOWN (296)",
		"UNKNOWN (297)",
		"UNKNOWN (298)",
		"UNKNOWN (299)",
		"UNKNOWN (300)",
		"UNKNOWN (301)",
		"UNKNOWN (302)",
		"UNKNOWN (303)",
		"UNKNOWN (304)",
		"UNKNOWN (305)",
		"WM_CTLCOLORMSGBOX",
		"WM_CTLCOLOREDIT",
		"WM_CTLCOLORLISTBOX",
		"WM_CTLCOLORBTN",
		"WM_CTLCOLORDLG",
		"WM_CTLCOLORSCROLLBAR",
		"WM_CTLCOLORSTATIC",
		"UNKNOWN (313)",
		"UNKNOWN (314)",
		"UNKNOWN (315)",
		"UNKNOWN (316)",
		"UNKNOWN (317)",
		"UNKNOWN (318)",
		"UNKNOWN (319)",
		"UNKNOWN (320)",
		"UNKNOWN (321)",
		"UNKNOWN (322)",
		"UNKNOWN (323)",
		"UNKNOWN (324)",
		"UNKNOWN (325)",
		"UNKNOWN (326)",
		"UNKNOWN (327)",
		"UNKNOWN (328)",
		"UNKNOWN (329)",
		"UNKNOWN (330)",
		"UNKNOWN (331)",
		"UNKNOWN (332)",
		"UNKNOWN (333)",
		"UNKNOWN (334)",
		"UNKNOWN (335)",
		"UNKNOWN (336)",
		"UNKNOWN (337)",
		"UNKNOWN (338)",
		"UNKNOWN (339)",
		"UNKNOWN (340)",
		"UNKNOWN (341)",
		"UNKNOWN (342)",
		"UNKNOWN (343)",
		"UNKNOWN (344)",
		"UNKNOWN (345)",
		"UNKNOWN (346)",
		"UNKNOWN (347)",
		"UNKNOWN (348)",
		"UNKNOWN (349)",
		"UNKNOWN (350)",
		"UNKNOWN (351)",
		"UNKNOWN (352)",
		"UNKNOWN (353)",
		"UNKNOWN (354)",
		"UNKNOWN (355)",
		"UNKNOWN (356)",
		"UNKNOWN (357)",
		"UNKNOWN (358)",
		"UNKNOWN (359)",
		"UNKNOWN (360)",
		"UNKNOWN (361)",
		"UNKNOWN (362)",
		"UNKNOWN (363)",
		"UNKNOWN (364)",
		"UNKNOWN (365)",
		"UNKNOWN (366)",
		"UNKNOWN (367)",
		"UNKNOWN (368)",
		"UNKNOWN (369)",
		"UNKNOWN (370)",
		"UNKNOWN (371)",
		"UNKNOWN (372)",
		"UNKNOWN (373)",
		"UNKNOWN (374)",
		"UNKNOWN (375)",
		"UNKNOWN (376)",
		"UNKNOWN (377)",
		"UNKNOWN (378)",
		"UNKNOWN (379)",
		"UNKNOWN (380)",
		"UNKNOWN (381)",
		"UNKNOWN (382)",
		"UNKNOWN (383)",
		"UNKNOWN (384)",
		"UNKNOWN (385)",
		"UNKNOWN (386)",
		"UNKNOWN (387)",
		"UNKNOWN (388)",
		"UNKNOWN (389)",
		"UNKNOWN (390)",
		"UNKNOWN (391)",
		"UNKNOWN (392)",
		"UNKNOWN (393)",
		"UNKNOWN (394)",
		"UNKNOWN (395)",
		"UNKNOWN (396)",
		"UNKNOWN (397)",
		"UNKNOWN (398)",
		"UNKNOWN (399)",
		"UNKNOWN (400)",
		"UNKNOWN (401)",
		"UNKNOWN (402)",
		"UNKNOWN (403)",
		"UNKNOWN (404)",
		"UNKNOWN (405)",
		"UNKNOWN (406)",
		"UNKNOWN (407)",
		"UNKNOWN (408)",
		"UNKNOWN (409)",
		"UNKNOWN (410)",
		"UNKNOWN (411)",
		"UNKNOWN (412)",
		"UNKNOWN (413)",
		"UNKNOWN (414)",
		"UNKNOWN (415)",
		"UNKNOWN (416)",
		"UNKNOWN (417)",
		"UNKNOWN (418)",
		"UNKNOWN (419)",
		"UNKNOWN (420)",
		"UNKNOWN (421)",
		"UNKNOWN (422)",
		"UNKNOWN (423)",
		"UNKNOWN (424)",
		"UNKNOWN (425)",
		"UNKNOWN (426)",
		"UNKNOWN (427)",
		"UNKNOWN (428)",
		"UNKNOWN (429)",
		"UNKNOWN (430)",
		"UNKNOWN (431)",
		"UNKNOWN (432)",
		"UNKNOWN (433)",
		"UNKNOWN (434)",
		"UNKNOWN (435)",
		"UNKNOWN (436)",
		"UNKNOWN (437)",
		"UNKNOWN (438)",
		"UNKNOWN (439)",
		"UNKNOWN (440)",
		"UNKNOWN (441)",
		"UNKNOWN (442)",
		"UNKNOWN (443)",
		"UNKNOWN (444)",
		"UNKNOWN (445)",
		"UNKNOWN (446)",
		"UNKNOWN (447)",
		"UNKNOWN (448)",
		"UNKNOWN (449)",
		"UNKNOWN (450)",
		"UNKNOWN (451)",
		"UNKNOWN (452)",
		"UNKNOWN (453)",
		"UNKNOWN (454)",
		"UNKNOWN (455)",
		"UNKNOWN (456)",
		"UNKNOWN (457)",
		"UNKNOWN (458)",
		"UNKNOWN (459)",
		"UNKNOWN (460)",
		"UNKNOWN (461)",
		"UNKNOWN (462)",
		"UNKNOWN (463)",
		"UNKNOWN (464)",
		"UNKNOWN (465)",
		"UNKNOWN (466)",
		"UNKNOWN (467)",
		"UNKNOWN (468)",
		"UNKNOWN (469)",
		"UNKNOWN (470)",
		"UNKNOWN (471)",
		"UNKNOWN (472)",
		"UNKNOWN (473)",
		"UNKNOWN (474)",
		"UNKNOWN (475)",
		"UNKNOWN (476)",
		"UNKNOWN (477)",
		"UNKNOWN (478)",
		"UNKNOWN (479)",
		"UNKNOWN (480)",
		"UNKNOWN (481)",
		"UNKNOWN (482)",
		"UNKNOWN (483)",
		"UNKNOWN (484)",
		"UNKNOWN (485)",
		"UNKNOWN (486)",
		"UNKNOWN (487)",
		"UNKNOWN (488)",
		"UNKNOWN (489)",
		"UNKNOWN (490)",
		"UNKNOWN (491)",
		"UNKNOWN (492)",
		"UNKNOWN (493)",
		"UNKNOWN (494)",
		"UNKNOWN (495)",
		"UNKNOWN (496)",
		"UNKNOWN (497)",
		"UNKNOWN (498)",
		"UNKNOWN (499)",
		"UNKNOWN (500)",
		"UNKNOWN (501)",
		"UNKNOWN (502)",
		"UNKNOWN (503)",
		"UNKNOWN (504)",
		"UNKNOWN (505)",
		"UNKNOWN (506)",
		"UNKNOWN (507)",
		"UNKNOWN (508)",
		"UNKNOWN (509)",
		"UNKNOWN (510)",
		"UNKNOWN (511)",
		"WM_MOUSEMOVE",
		"WM_LBUTTONDOWN",
		"WM_LBUTTONUP",
		"WM_LBUTTONDBLCLK",
		"WM_RBUTTONDOWN",
		"WM_RBUTTONUP",
		"WM_RBUTTONDBLCLK",
		"WM_MBUTTONDOWN",
		"WM_MBUTTONUP",
		"WM_MOUSELAST",
		"WM_MOUSEWHEEL",
		"WM_XBUTTONDOWN",
		"WM_XBUTTONUP",
		"UNKNOWN (525)",
		"UNKNOWN (526)",
		"UNKNOWN (527)",
		"WM_PARENTNOTIFY",
		"WM_ENTERMENULOOP",
		"WM_EXITMENULOOP",
		"WM_NEXTMENU",
		"WM_SIZING",
		"WM_CAPTURECHANGED",
		"WM_MOVING",
		"UNKNOWN (535)",
		"WM_POWERBROADCAST",
		"WM_DEVICECHANGE",
		"UNKNOWN (538)",
		"UNKNOWN (539)",
		"UNKNOWN (540)",
		"UNKNOWN (541)",
		"UNKNOWN (542)",
		"UNKNOWN (543)",
		"WM_MDICREATE",
		"WM_MDIDESTROY",
		"WM_MDIACTIVATE",
		"WM_MDIRESTORE",
		"WM_MDINEXT",
		"WM_MDIMAXIMIZE",
		"WM_MDITILE",
		"WM_MDICASCADE",
		"WM_MDIICONARRANGE",
		"WM_MDIGETACTIVE",
		"UNKNOWN (554)",
		"UNKNOWN (555)",
		"UNKNOWN (556)",
		"UNKNOWN (557)",
		"UNKNOWN (558)",
		"UNKNOWN (559)",
		"WM_MDISETMENU",
		"WM_ENTERSIZEMOVE",
		"WM_EXITSIZEMOVE",
		"WM_DROPFILES",
		"WM_MDIREFRESHMENU",
		"UNKNOWN (565)",
		"UNKNOWN (566)",
		"UNKNOWN (567)",
		"UNKNOWN (568)",
		"UNKNOWN (569)",
		"UNKNOWN (570)",
		"UNKNOWN (571)",
		"UNKNOWN (572)",
		"UNKNOWN (573)",
		"UNKNOWN (574)",
		"UNKNOWN (575)",
		"UNKNOWN (576)",
		"UNKNOWN (577)",
		"UNKNOWN (578)",
		"UNKNOWN (579)",
		"UNKNOWN (580)",
		"UNKNOWN (581)",
		"UNKNOWN (582)",
		"UNKNOWN (583)",
		"UNKNOWN (584)",
		"UNKNOWN (585)",
		"UNKNOWN (586)",
		"UNKNOWN (587)",
		"UNKNOWN (588)",
		"UNKNOWN (589)",
		"UNKNOWN (590)",
		"UNKNOWN (591)",
		"UNKNOWN (592)",
		"UNKNOWN (593)",
		"UNKNOWN (594)",
		"UNKNOWN (595)",
		"UNKNOWN (596)",
		"UNKNOWN (597)",
		"UNKNOWN (598)",
		"UNKNOWN (599)",
		"UNKNOWN (600)",
		"UNKNOWN (601)",
		"UNKNOWN (602)",
		"UNKNOWN (603)",
		"UNKNOWN (604)",
		"UNKNOWN (605)",
		"UNKNOWN (606)",
		"UNKNOWN (607)",
		"UNKNOWN (608)",
		"UNKNOWN (609)",
		"UNKNOWN (610)",
		"UNKNOWN (611)",
		"UNKNOWN (612)",
		"UNKNOWN (613)",
		"UNKNOWN (614)",
		"UNKNOWN (615)",
		"UNKNOWN (616)",
		"UNKNOWN (617)",
		"UNKNOWN (618)",
		"UNKNOWN (619)",
		"UNKNOWN (620)",
		"UNKNOWN (621)",
		"UNKNOWN (622)",
		"UNKNOWN (623)",
		"UNKNOWN (624)",
		"UNKNOWN (625)",
		"UNKNOWN (626)",
		"UNKNOWN (627)",
		"UNKNOWN (628)",
		"UNKNOWN (629)",
		"UNKNOWN (630)",
		"UNKNOWN (631)",
		"UNKNOWN (632)",
		"UNKNOWN (633)",
		"UNKNOWN (634)",
		"UNKNOWN (635)",
		"UNKNOWN (636)",
		"UNKNOWN (637)",
		"UNKNOWN (638)",
		"UNKNOWN (639)",
		"UNKNOWN (640)",
		"UNKNOWN (641)",
		"UNKNOWN (642)",
		"UNKNOWN (643)",
		"UNKNOWN (644)",
		"UNKNOWN (645)",
		"UNKNOWN (646)",
		"UNKNOWN (647)",
		"UNKNOWN (648)",
		"UNKNOWN (649)",
		"UNKNOWN (650)",
		"UNKNOWN (651)",
		"UNKNOWN (652)",
		"UNKNOWN (653)",
		"UNKNOWN (654)",
		"UNKNOWN (655)",
		"UNKNOWN (656)",
		"UNKNOWN (657)",
		"UNKNOWN (658)",
		"UNKNOWN (659)",
		"UNKNOWN (660)",
		"UNKNOWN (661)",
		"UNKNOWN (662)",
		"UNKNOWN (663)",
		"UNKNOWN (664)",
		"UNKNOWN (665)",
		"UNKNOWN (666)",
		"UNKNOWN (667)",
		"UNKNOWN (668)",
		"UNKNOWN (669)",
		"UNKNOWN (670)",
		"UNKNOWN (671)",
		"UNKNOWN (672)",
		"WM_MOUSEHOVER",
		"WM_NCMOUSELEAVE",
		"WM_MOUSELEAVE",
		"UNKNOWN (676)",
		"UNKNOWN (677)",
		"UNKNOWN (678)",
		"UNKNOWN (679)",
		"UNKNOWN (680)",
		"UNKNOWN (681)",
		"UNKNOWN (682)",
		"UNKNOWN (683)",
		"UNKNOWN (684)",
		"UNKNOWN (685)",
		"UNKNOWN (686)",
		"UNKNOWN (687)",
		"UNKNOWN (688)",
		"UNKNOWN (689)",
		"UNKNOWN (690)",
		"UNKNOWN (691)",
		"UNKNOWN (692)",
		"UNKNOWN (693)",
		"UNKNOWN (694)",
		"UNKNOWN (695)",
		"UNKNOWN (696)",
		"UNKNOWN (697)",
		"UNKNOWN (698)",
		"UNKNOWN (699)",
		"UNKNOWN (700)",
		"UNKNOWN (701)",
		"UNKNOWN (702)",
		"UNKNOWN (703)",
		"UNKNOWN (704)",
		"UNKNOWN (705)",
		"UNKNOWN (706)",
		"UNKNOWN (707)",
		"UNKNOWN (708)",
		"UNKNOWN (709)",
		"UNKNOWN (710)",
		"UNKNOWN (711)",
		"UNKNOWN (712)",
		"UNKNOWN (713)",
		"UNKNOWN (714)",
		"UNKNOWN (715)",
		"UNKNOWN (716)",
		"UNKNOWN (717)",
		"UNKNOWN (718)",
		"UNKNOWN (719)",
		"UNKNOWN (720)",
		"UNKNOWN (721)",
		"UNKNOWN (722)",
		"UNKNOWN (723)",
		"UNKNOWN (724)",
		"UNKNOWN (725)",
		"UNKNOWN (726)",
		"UNKNOWN (727)",
		"UNKNOWN (728)",
		"UNKNOWN (729)",
		"UNKNOWN (730)",
		"UNKNOWN (731)",
		"UNKNOWN (732)",
		"UNKNOWN (733)",
		"UNKNOWN (734)",
		"UNKNOWN (735)",
		"UNKNOWN (736)",
		"UNKNOWN (737)",
		"UNKNOWN (738)",
		"UNKNOWN (739)",
		"UNKNOWN (740)",
		"UNKNOWN (741)",
		"UNKNOWN (742)",
		"UNKNOWN (743)",
		"UNKNOWN (744)",
		"UNKNOWN (745)",
		"UNKNOWN (746)",
		"UNKNOWN (747)",
		"UNKNOWN (748)",
		"UNKNOWN (749)",
		"UNKNOWN (750)",
		"UNKNOWN (751)",
		"UNKNOWN (752)",
		"UNKNOWN (753)",
		"UNKNOWN (754)",
		"UNKNOWN (755)",
		"UNKNOWN (756)",
		"UNKNOWN (757)",
		"UNKNOWN (758)",
		"UNKNOWN (759)",
		"UNKNOWN (760)",
		"UNKNOWN (761)",
		"UNKNOWN (762)",
		"UNKNOWN (763)",
		"UNKNOWN (764)",
		"UNKNOWN (765)",
		"UNKNOWN (766)",
		"UNKNOWN (767)",
		"WM_CUT",
		"WM_COPY",
		"WM_PASTE",
		"WM_CLEAR",
		"WM_UNDO",
		"WM_RENDERFORMAT",
		"WM_RENDERALLFORMATS",
		"WM_DESTROYCLIPBOARD",
		"WM_DRAWCLIPBOARD",
		"WM_PAINTCLIPBOARD",
		"WM_VSCROLLCLIPBOARD",
		"WM_SIZECLIPBOARD",
		"WM_ASKCBFORMATNAME",
		"WM_CHANGECBCHAIN",
		"WM_HSCROLLCLIPBOARD",
		"WM_QUERYNEWPALETTE",
		"WM_PALETTEISCHANGING",
		"WM_PALETTECHANGED",
		"WM_HOTKEY",
		"UNKNOWN (787)",
		"UNKNOWN (788)",
		"UNKNOWN (789)",
		"UNKNOWN (790)",
		"WM_PRINT",
		"WM_PRINTCLIENT",
		"UNKNOWN (793)",
		"UNKNOWN (794)",
		"UNKNOWN (795)",
		"UNKNOWN (796)",
		"UNKNOWN (797)",
		"UNKNOWN (798)",
		"UNKNOWN (799)",
		"UNKNOWN (800)",
		"UNKNOWN (801)",
		"UNKNOWN (802)",
		"UNKNOWN (803)",
		"UNKNOWN (804)",
		"UNKNOWN (805)",
		"UNKNOWN (806)",
		"UNKNOWN (807)",
		"UNKNOWN (808)",
		"UNKNOWN (809)",
		"UNKNOWN (810)",
		"UNKNOWN (811)",
		"UNKNOWN (812)",
		"UNKNOWN (813)",
		"UNKNOWN (814)",
		"UNKNOWN (815)",
		"UNKNOWN (816)",
		"UNKNOWN (817)",
		"UNKNOWN (818)",
		"UNKNOWN (819)",
		"UNKNOWN (820)",
		"UNKNOWN (821)",
		"UNKNOWN (822)",
		"UNKNOWN (823)",
		"UNKNOWN (824)",
		"UNKNOWN (825)",
		"UNKNOWN (826)",
		"UNKNOWN (827)",
		"UNKNOWN (828)",
		"UNKNOWN (829)",
		"UNKNOWN (830)",
		"UNKNOWN (831)",
		"UNKNOWN (832)",
		"UNKNOWN (833)",
		"UNKNOWN (834)",
		"UNKNOWN (835)",
		"UNKNOWN (836)",
		"UNKNOWN (837)",
		"UNKNOWN (838)",
		"UNKNOWN (839)",
		"UNKNOWN (840)",
		"UNKNOWN (841)",
		"UNKNOWN (842)",
		"UNKNOWN (843)",
		"UNKNOWN (844)",
		"UNKNOWN (845)",
		"UNKNOWN (846)",
		"UNKNOWN (847)",
		"UNKNOWN (848)",
		"UNKNOWN (849)",
		"UNKNOWN (850)",
		"UNKNOWN (851)",
		"UNKNOWN (852)",
		"UNKNOWN (853)",
		"UNKNOWN (854)",
		"UNKNOWN (855)",
		"WM_HANDHELDFIRST",
		"UNKNOWN (857)",
		"UNKNOWN (858)",
		"UNKNOWN (859)",
		"UNKNOWN (860)",
		"UNKNOWN (861)",
		"UNKNOWN (862)",
		"WM_HANDHELDLAST",
		"WM_AFXFIRST",
		"UNKNOWN (865)",
		"UNKNOWN (866)",
		"UNKNOWN (867)",
		"UNKNOWN (868)",
		"UNKNOWN (869)",
		"UNKNOWN (870)",
		"UNKNOWN (871)",
		"UNKNOWN (872)",
		"UNKNOWN (873)",
		"UNKNOWN (874)",
		"UNKNOWN (875)",
		"UNKNOWN (876)",
		"UNKNOWN (877)",
		"UNKNOWN (878)",
		"UNKNOWN (879)",
		"UNKNOWN (880)",
		"UNKNOWN (881)",
		"UNKNOWN (882)",
		"UNKNOWN (883)",
		"UNKNOWN (884)",
		"UNKNOWN (885)",
		"UNKNOWN (886)",
		"UNKNOWN (887)",
		"UNKNOWN (888)",
		"UNKNOWN (889)",
		"UNKNOWN (890)",
		"UNKNOWN (891)",
		"UNKNOWN (892)",
		"UNKNOWN (893)",
		"UNKNOWN (894)",
		"WM_AFXLAST",
		"WM_PENWINFIRST",
		"UNKNOWN (897)",
		"UNKNOWN (898)",
		"UNKNOWN (899)",
		"UNKNOWN (900)",
		"UNKNOWN (901)",
		"UNKNOWN (902)",
		"UNKNOWN (903)",
		"UNKNOWN (904)",
		"UNKNOWN (905)",
		"UNKNOWN (906)",
		"UNKNOWN (907)",
		"UNKNOWN (908)",
		"UNKNOWN (909)",
		"UNKNOWN (910)",
		"WM_PENWINLAST",
		"UNKNOWN (912)",
		"UNKNOWN (913)",
		"UNKNOWN (914)",
		"UNKNOWN (915)",
		"UNKNOWN (916)",
		"UNKNOWN (917)",
		"UNKNOWN (918)",
		"UNKNOWN (919)",
		"UNKNOWN (920)",
		"UNKNOWN (921)",
		"UNKNOWN (922)",
		"UNKNOWN (923)",
		"UNKNOWN (924)",
		"UNKNOWN (925)",
		"UNKNOWN (926)",
		"UNKNOWN (927)",
		"UNKNOWN (928)",
		"UNKNOWN (929)",
		"UNKNOWN (930)",
		"UNKNOWN (931)",
		"UNKNOWN (932)",
		"UNKNOWN (933)",
		"UNKNOWN (934)",
		"UNKNOWN (935)",
		"UNKNOWN (936)",
		"UNKNOWN (937)",
		"UNKNOWN (938)",
		"UNKNOWN (939)",
		"UNKNOWN (940)",
		"UNKNOWN (941)",
		"UNKNOWN (942)",
		"UNKNOWN (943)",
		"UNKNOWN (944)",
		"UNKNOWN (945)",
		"UNKNOWN (946)",
		"UNKNOWN (947)",
		"UNKNOWN (948)",
		"UNKNOWN (949)",
		"UNKNOWN (950)",
		"UNKNOWN (951)",
		"UNKNOWN (952)",
		"UNKNOWN (953)",
		"UNKNOWN (954)",
		"UNKNOWN (955)",
		"UNKNOWN (956)",
		"UNKNOWN (957)",
		"UNKNOWN (958)",
		"UNKNOWN (959)",
		"UNKNOWN (960)",
		"UNKNOWN (961)",
		"UNKNOWN (962)",
		"UNKNOWN (963)",
		"UNKNOWN (964)",
		"UNKNOWN (965)",
		"UNKNOWN (966)",
		"UNKNOWN (967)",
		"UNKNOWN (968)",
		"UNKNOWN (969)",
		"UNKNOWN (970)",
		"UNKNOWN (971)",
		"UNKNOWN (972)",
		"UNKNOWN (973)",
		"UNKNOWN (974)",
		"UNKNOWN (975)",
		"UNKNOWN (976)",
		"UNKNOWN (977)",
		"UNKNOWN (978)",
		"UNKNOWN (979)",
		"UNKNOWN (980)",
		"UNKNOWN (981)",
		"UNKNOWN (982)",
		"UNKNOWN (983)",
		"UNKNOWN (984)",
		"UNKNOWN (985)",
		"UNKNOWN (986)",
		"UNKNOWN (987)",
		"UNKNOWN (988)",
		"UNKNOWN (989)",
		"UNKNOWN (990)",
		"UNKNOWN (991)",
		"UNKNOWN (992)",
		"UNKNOWN (993)",
		"UNKNOWN (994)",
		"UNKNOWN (995)",
		"UNKNOWN (996)",
		"UNKNOWN (997)",
		"UNKNOWN (998)",
		"UNKNOWN (999)",
		"UNKNOWN (1000)",
		"UNKNOWN (1001)",
		"UNKNOWN (1002)",
		"UNKNOWN (1003)",
		"UNKNOWN (1004)",
		"UNKNOWN (1005)",
		"UNKNOWN (1006)",
		"UNKNOWN (1007)",
		"UNKNOWN (1008)",
		"UNKNOWN (1009)",
		"UNKNOWN (1010)",
		"UNKNOWN (1011)",
		"UNKNOWN (1012)",
		"UNKNOWN (1013)",
		"UNKNOWN (1014)",
		"UNKNOWN (1015)",
		"UNKNOWN (1016)",
		"UNKNOWN (1017)",
		"UNKNOWN (1018)",
		"UNKNOWN (1019)",
		"UNKNOWN (1020)",
		"UNKNOWN (1021)",
		"UNKNOWN (1022)",
		"UNKNOWN (1023)",
		"WM_USER"
	};
	
	const size_t num_names = sizeof(message_names) / sizeof(message_names[0]);

	const wm::Event *makeKey(
		wm::Window& window,
		UINT message,
		WPARAM wparam,
		LPARAM lparam)
	{
		return new wm::KeyEvent(
			window, 
			wm::win32::mapVirtualKeyCode(wparam),
			message==WM_KEYDOWN);
	}

	const wm::Event *makeButton(
		wm::Window& window,
		UINT message,
		WPARAM wparam,
		LPARAM lparam)
	{
		unsigned int x = LOWORD(lparam),
					 y = HIWORD(lparam);

		int button = 0;
		bool state = false;
		switch(message)
		{
		case WM_LBUTTONDOWN:
			button = 1; state = true;
			break;
		case WM_LBUTTONUP:
			button = 1; state = false;
			break;

		case WM_MBUTTONDOWN:
			button = 2; state = true;
			break;
		case WM_MBUTTONUP:
			button = 2; state = false;
			break;

		case WM_RBUTTONDOWN:
			button = 3; state = true;
			break;
		case WM_RBUTTONUP:
			button = 3; state = false;
			break;

		case WM_XBUTTONDOWN:
			button = 7 + GET_XBUTTON_WPARAM(wparam);
			state = true;
			break;
		case WM_XBUTTONUP:
			button = 7 + GET_XBUTTON_WPARAM(wparam);
			state = false;
			break;

		case WM_MOUSEWHEEL:
			button = GET_WHEEL_DELTA_WPARAM(wparam) < 0 
				? 4 : 5;
			state = true;
			break;

		case WM_MOUSEHWHEEL:
			button = GET_WHEEL_DELTA_WPARAM(wparam) < 0
				? 6 : 7;
			state = true;
			break;

		}

		return new wm::ButtonEvent(window, x, y, button, state);
	}

	const wm::Event *makeFocus(
		wm::Window& window,
		UINT message,
		WPARAM wparam,
		LPARAM lparam)
	{
		return new wm::FocusEvent(window, message == WM_SETFOCUS ? true : false);
	}

	const wm::Event *makeClose(
		wm::Window& window,
		UINT message,
		WPARAM wparam,
		LPARAM lparam)
	{
		return new wm::CloseEvent(window);
	}

	const wm::Event *makeChar(
		wm::Window& window,
		UINT message,
		WPARAM wparam,
		LPARAM lparam)
	{
		boost::uint16_t lo = wparam & 0xFFFF;
		boost::uint16_t hi = (wparam & 0xFFFF0000) >> 16;
		boost::uint32_t codepoint = 0;

		// Convert from UTF-16 to Unicode codepoint
		if(lo < 0xD8000 || lo > 0xDFFF) codepoint = lo;
		else if(lo < 0xD800 || lo > 0xDBFF) throw wm::Exception("Win32 WM_CHAR message contains invalid UTF-16");
		else if(hi < 0xDC00 || hi > 0xDFFF) throw wm::Exception("Win32 WM_CHAR message contains invalid UTF-16");
		else codepoint = ((lo & 0x3FF) << 10) | (hi & 0x3FF) + 0x10000;

		return new wm::CharacterEvent(window, codepoint);
	}
}

namespace wm
{
	namespace win32
	{
		const Event* fromWin32Event(
			Window& window,
			UINT message,
			WPARAM wparam,
			LPARAM lparam)
		{
			typedef const Event*
				(FactoryFunc) (
					Window& window,
					UINT message,
					WPARAM wparam,
					LPARAM lparam);

			static struct temp_t
			{
				temp_t()
				{
					map[WM_KEYUP] = &makeKey;
					map[WM_KEYDOWN] = &makeKey;

					map[WM_CHAR] = &makeChar;

					map[WM_LBUTTONDOWN]= &makeButton;
					map[WM_LBUTTONUP]= &makeButton;
					map[WM_MBUTTONDOWN]= &makeButton;
					map[WM_MBUTTONUP]= &makeButton;
					map[WM_RBUTTONDOWN]= &makeButton;
					map[WM_RBUTTONUP]= &makeButton;
					map[WM_XBUTTONDOWN]= &makeButton;
					map[WM_XBUTTONUP]= &makeButton;
					map[WM_MOUSEWHEEL]= &makeButton;
					map[WM_MOUSEHWHEEL]= &makeButton;

					map[WM_SETFOCUS] = &makeFocus;
					map[WM_KILLFOCUS] = &makeFocus;

					map[WM_CLOSE] = &makeClose;
				}

				typedef std::map<UINT, FactoryFunc*> map_t;
				map_t map;
			} registry;

			typedef temp_t::map_t::const_iterator iterator;
			iterator i = registry.map.find(message);
			
			if(i == registry.map.end())
			{
				if(message < num_names)
				{
					switch(message)
					{
					case WM_MOUSEMOVE:
					case WM_NCMOUSEMOVE:
					case WM_SETCURSOR:
					case WM_NCHITTEST:
						break;
					default:
						//std::cout << "Unhandled win32 message: " << message_names[message] << std::endl;
						break;
					}
				}
				else
					std::cout << "Unknown message: " << message << std::endl;
				return 0;
			}

			FactoryFunc* factory = i->second;
			if(factory) return factory(window, message, wparam, lparam);

			return 0;
		}
	}
}
