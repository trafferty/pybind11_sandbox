import dif

eng_list = [dif.Sim_ImgEngine("SimEngine", True), dif.PCO_ImgEngine("PCOEngine", True)]

config = {}
config["device_num"] =   0
config["exposure_us"] =  20000
config["delay_us"] =     0.0
config["conversion_factor"] = 1.0
config["width_px"] =     1392
config["height_px"] =    1040
config["framerate_hz"] = 30
config["offset_y_px"] =  0
config["offset_x_px"] =  0
config["pel_size_um"] =  28
config["binh"] =         1
config["binv"] =         1
config["trigger_mode"] = 2
config["doubleImage_mode"] = 0
config["timestamp_mode"] = 0

for eng in eng_list:
    print("******** Testing Engine: %s" % eng.getName())
    eng.init(config)

    eng.cur_info()
    eng.device_info()

    print("getPelSize_um: %f" % eng.getPelSize_um())

    print("img_idx: %d", eng.img_idx)

    print("RECORD_MODE_OFF: %d" % int(eng.RECORD_MODE_OFF))
    print("RECORD_MODE_ON: %d" % int(eng.RECORD_MODE_ON))
    print("COLOR_MODE_DEFAULT: %d" % int(eng.COLOR_MODE_DEFAULT))
    print("COLOR_MODE_RGB: %d" % int(eng.COLOR_MODE_RGB))
    print("COLOR_MODE_RAW8: %d" % int(eng.COLOR_MODE_RAW8))

    print("getColorMode before: %d" % eng.getColorMode())
    eng.setColorMode(eng.COLOR_MODE_RAW8)
    print("getColorMode after : %d" % eng.getColorMode())

    m = eng.next()
    rows, cols = m.shape
    print("matrix shape: %dx%d" %  (rows, cols))
    print("matrix max: ", m.max())
    print("matrix min: ", m.min())
    print("matrix sum: ", m.sum())

    for idx in range(10):
        print("m[0][%d]: %d" % (idx, m[0][idx]))

    # for idx in range( rows):
    #     print("m[%d][%d]: %d" % (cols-1, idx, m[cols-1][idx]))

    w,h = eng.getImageSize()
    print("getImageSize: width: %d, height: %d" % (w,h))
