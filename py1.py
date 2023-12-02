with open("build/text_section.bin", "rb") as f: 
    print("[Wrapper]\nmoduleMatches = 0x867317DE,0x6237F45C,0x90112329,0x6237f45c\n\n0x02F37154 = b _onStart\n\n0x104D4DD8 = .uint _Code\n0x104D4DDC = .uint 0x0\n\n.origin = codecave\n\n# .text\n_Code:"); 
    print(".uint", end=" 0x"); 
    [print("%02x" % val, end="" if i % 4 != 0 else "\n.uint 0x") 
    for i, val in enumerate(f.read(), 1)]; 
    print("00000000\n\n_onStart:\nbctrl\nlis r12, _Code@ha\naddi r12, r12, _Code@l\nmtctr r12\nbctrl\nb 0x02F37158")