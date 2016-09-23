#!/usr/bin/env python

from rootdrawing import *
from track_drawer import *
from helper import *
from math import pi, cos, sin, tan, sinh, asin, atan2, exp
from numpy import genfromtxt

# ______________________________________________________________________________
# Functions
quadsum = lambda x,y: sqrt(x*x + y*y)

# ______________________________________________________________________________
# Globals

# Load trigger tower map
# ttmap = json.load(open("../data/trigger_sector_map_new.json"), object_pairs_hook=convert_key_to_int)

# Load module vertices
vertexmap = json.load(open("/Users/olmo/Dropbox/16_07_FERMILAB_summer_school/CMStrigger/script/detector_drawer/data/module_vertices.json"), object_pairs_hook=convert_key_to_int)

moduleIds_set = set()
tpolylines_xy = {}
tpolylines_rz = {}
for moduleId, xyz in vertexmap.iteritems():
    if moduleId < 0:  continue
    moduleIds_set.add(moduleId)

    xy, rz = [], []
    for i in xrange(4):
        c = (xyz[3*i+0], xyz[3*i+1])
        if c not in xy:  xy.append(c)
        c = (xyz[3*i+2], quadsum(xyz[3*i], xyz[3*i+1]))
        if c not in rz:  rz.append(c)

    for coords, tpolylines in [(xy, tpolylines_xy), (rz, tpolylines_rz)]:
        x, y = [], []
        for c in coords:
            x.append(c[0])
            y.append(c[1])
        if len(xy) == 4:  # close the poly line in case it's made of 4 distinct points
            x.append(x[0])
            y.append(y[0])
        tpolylines[moduleId] = TPolyLine(len(x), array('d', x), array('d', y))

#count = 0
#for tt in xrange(48):
#    tt_moduleIds = ttmap[tt]
#    count += len(tt_moduleIds)
#
#print "Number of unique modules: %i" % len(moduleIds_set)
#print "Number of modules incl. duplicates: %i" % count

nbinsx, xmin, xmax = 500, -120., 120.
nbinsy, ymin, ymax = 500, -120., 120.
nbinsr, rmin, rmax = 500,    0., 120.
nbinsz, zmin, zmax = 500, -300., 300.

tline2 = TLine()
tline2.SetLineColor(kGray)
tline2.SetLineStyle(7)
tline2.SetLineWidth(1)

# ______________________________________________________________________________
# Drawer
def drawer_book(options):
    histos = {}

    hname = "xy1"
    histos[hname] = TH2F(hname, "Barrel x-y projection; x [cm]; y [cm]", nbinsx, xmin, xmax, nbinsy, ymin, ymax)
    hname = "xy2"
    histos[hname] = TH2F(hname, "Endcap x-y projection; x [cm]; y [cm]", nbinsx, xmin, xmax, nbinsy, ymin, ymax)
    hname = "rz"
    histos[hname] = TH2F(hname, "Full r-z projection; z [cm]; r [cm]", nbinsz, zmin, zmax, nbinsy, rmin, rmax)

    # Style
    for hname, h in histos.iteritems():
        h.style = 0; h.logx = options.logx; h.logy = options.logy
    donotdelete.append(histos)
    return histos

# ______________________________________________________________________________
def drawer_project(tree, histos, options):
    return

# ______________________________________________________________________________
def drawer_draw(histos, options, debug=False):

    modules_file = open(options.infile)
    modules_list = modules_file.readlines()
    tag = options.filetag

    min_pt, max_vz, max_rho = options.min_pt, options.max_vz, options.max_rho
    tt = options.tt


    # Make frames
    h = histos["xy1"]
    h.c1 = TCanvas("c_"+h.GetName(), "", 800, 800)
    h.SetStats(0); h.Draw()
    h.GetYaxis().SetTitleOffset(1.4)

    h = histos["xy2"]
    h.c1 = TCanvas("c_"+h.GetName(), "", 800, 800)
    h.SetStats(0); h.Draw()
    h.GetYaxis().SetTitleOffset(1.4)

    h = histos["rz"]
    h.c1 = TCanvas("c_"+h.GetName(), "", 1100, 550)
    h.c1.SetLeftMargin(0.05); h.GetYaxis().SetTitleOffset(0.6)
    h.SetStats(0); h.Draw()

    def style_tpolyline(l, module, colorized = 0):
        l.SetLineStyle(1)
        l.SetLineWidth(2)
        if colorized:
            #if isPSModule(module):
            #    l.SetLineColor(palette[0])
            #else:
            #    l.SetLineColor(palette[1])
            # l.SetLineColor(palette[colorized])
            l.SetLineColor(colorized)

        else:
            l.SetLineColor(kGray)

    def doit():
        if 5 <= lay <= 10:  # Barrel
            histos["xy1"].c1.cd()
            tpolyline_xy.Draw()
            histos["rz"].c1.cd()
            tpolyline_rz.Draw()
        elif 11 <= lay <= 15 or 18 <= lay <= 22:  # Endcap
            histos["xy2"].c1.cd()
            tpolyline_xy.Draw()
            histos["rz"].c1.cd()
            tpolyline_rz.Draw()
        else:
            raise Exception("Unexpected moduleId: %i" % moduleId)

    # Draw polylines
    tpolylines = tpolylines_xy
    for moduleId in moduleIds_set:
        lay = decodeLayer(moduleId)
        tpolyline_xy = tpolylines_xy[moduleId]
        tpolyline_rz = tpolylines_rz[moduleId]
        style_tpolyline(tpolyline_xy, moduleId, 0)
        style_tpolyline(tpolyline_rz, moduleId, 0)
        doit()

    # Draw polylines for specific tower
    #Metti qua i tuoi module ID che vuoi disegnare
    # for moduleId in ttmap[tt]:
    colori = [1,2,3,4,5,6,7,8,9]

    for moduleId in modules_list:
        moduleId = int(moduleId)
        lay = decodeLayer(moduleId)
        tpolyline_xy = tpolylines_xy[moduleId]
        tpolyline_rz = tpolylines_rz[moduleId]
        style_tpolyline(tpolyline_xy, moduleId, colori[0])
        style_tpolyline(tpolyline_rz, moduleId, colori[0])
        doit()


    if options.file2 and not options.file3:
        find_TTN = re.search("_TT(.+?)_",options.file2)
        TTN = int(find_TTN.group(1))
        tag = "_tt%i" %TTN + tag
        modules_file2 = open(options.file2)
        modules_list2 = modules_file2.readlines()

        h_aux = TH1F("h_aux","h_aux",1,0,1)
        h_aux.SetLineColor(colori[0])

        h_aux2 = TH1F("h_aux2","h_aux2",1,0,1)
        h_aux2.SetLineColor(colori[1])

        h_aux3 = TH1F("h_aux2","h_aux2",1,0,1)
        h_aux3.SetLineColor(colori[2])

        leg = TLegend(0.85,0.1,0.97,0.3)
        leg.SetLineWidth(0);

        leg.AddEntry(h_aux,"TT%i"%options.tt,"l")
        leg.AddEntry(h_aux2,"TT%i"%TTN,"l")
        leg.AddEntry(h_aux3,"Intersection","l")



        for moduleId in modules_list2:
            moduleId = int(moduleId)
            lay = decodeLayer(moduleId)
            tpolyline_xy = tpolylines_xy[moduleId]
            tpolyline_rz = tpolylines_rz[moduleId]
            style_tpolyline(tpolyline_xy, moduleId, colori[1])
            style_tpolyline(tpolyline_rz, moduleId, colori[1])
            doit()

        list_intersection = list(set(modules_list2) & set(modules_list))

        for moduleId in list_intersection:
            moduleId = int(moduleId)
            lay = decodeLayer(moduleId)
            tpolyline_xy = tpolylines_xy[moduleId]
            tpolyline_rz = tpolylines_rz[moduleId]
            style_tpolyline(tpolyline_xy, moduleId, colori[2])
            style_tpolyline(tpolyline_rz, moduleId, colori[2])
            doit()

        histos["xy1"].c1.cd()
        leg.Draw("SAME")
        histos["xy2"].c1.cd()
        leg.Draw("SAME")
        histos["rz"].c1.cd()
        leg.Draw("SAME")

    if options.file2 and options.file3:
        TTN = [options.tt]

        find_TTN = re.search("_TT(.+?)_",options.file2)
        TTN.append(int(find_TTN.group(1)))
        find_TTN = re.search("_TT(.+?)_",options.file3)
        TTN.append(int(find_TTN.group(1)))

        tag = "_tt%i" %TTN[1] + "_tt%i" %TTN[2] + tag

        modules_file2 = open(options.file2)
        modules_list2 = modules_file2.readlines()

        modules_file3 = open(options.file3)
        modules_list3 = modules_file3.readlines()

        leg = TLegend(0.85,0.1,1,0.6)
        leg.SetLineWidth(0);
        leg.SetFillStyle(0);

        h_aux = []
        for i in range(7):
            h_aux.append(TH1F("h_aux"+str(i),"h_aux"+str(i),1,0,1))
            h_aux[i].SetLineColor(colori[i])
            if i<3:
                leg.AddEntry(h_aux[i],"TT%i"%TTN[i],"l")

        leg.AddEntry(h_aux[3],"TT" + str(TTN[0]) + " & TT" + str(TTN[1]),"l")
        leg.AddEntry(h_aux[4],"TT" + str(TTN[1]) + " & TT" + str(TTN[2]),"l")
        leg.AddEntry(h_aux[5],"TT" + str(TTN[0]) + " & TT" + str(TTN[2]),"l")
        leg.AddEntry(h_aux[6],"TT" + str(TTN[0]) + " & TT" + str(TTN[1]) + " & TT" + str(TTN[2]),"l")

        aux_list = list(set(modules_list2) - set(modules_list) - set(modules_list3))
        for moduleId in aux_list:
            moduleId = int(moduleId)
            lay = decodeLayer(moduleId)
            tpolyline_xy = tpolylines_xy[moduleId]
            tpolyline_rz = tpolylines_rz[moduleId]
            style_tpolyline(tpolyline_xy, moduleId, colori[1])
            style_tpolyline(tpolyline_rz, moduleId, colori[1])
            doit()


        aux_list = list(set(modules_list3) - set(modules_list) - set(modules_list2))
        for moduleId in aux_list:
            moduleId = int(moduleId)
            lay = decodeLayer(moduleId)
            tpolyline_xy = tpolylines_xy[moduleId]
            tpolyline_rz = tpolylines_rz[moduleId]
            style_tpolyline(tpolyline_xy, moduleId, colori[2])
            style_tpolyline(tpolyline_rz, moduleId, colori[2])
            doit()

        list_intersection = []
        list_intersection.append(list(set(modules_list2) & set(modules_list)))
        list_intersection.append(list(set(modules_list2) & set(modules_list3)))
        list_intersection.append(list(set(modules_list3) & set(modules_list)))
        list_intersection.append( list( set(modules_list2) & set(modules_list) & set(modules_list3) ) )
        print "Triple intersection for ", len(list_intersection[3]), "modules"


        for ii in range(4):
            for moduleId in list_intersection[ii]:
                moduleId = int(moduleId)
                lay = decodeLayer(moduleId)
                tpolyline_xy = tpolylines_xy[moduleId]
                tpolyline_rz = tpolylines_rz[moduleId]
                style_tpolyline(tpolyline_xy, moduleId, colori[ii+3])
                style_tpolyline(tpolyline_rz, moduleId, colori[ii+3])
                doit()

        histos["xy1"].c1.cd()
        leg.Draw("SAME")
        histos["xy2"].c1.cd()
        leg.Draw("SAME")
        histos["rz"].c1.cd()
        leg.Draw("SAME")

    if options.tracksfile:
        tag = tag + "_tracks"
        mydata = genfromtxt(options.tracksfile,skip_header=0,comments="#",delimiter=' ', dtype=None)
        histos["xy1"].c1.cd()
        leg = TLegend(0.15,0.7,0.35,0.9)

        for i,data in enumerate(mydata):
            # print "traccia"
            c = DrawTrack_xy1(data[0], data[1], data[2], data[3], data[4], data[5], data[6])
            donotdelete.append(c)
            c.SetLineWidth(3)
            if i<2:
                c.SetLineColor(6)
            else:
                c.SetLineColor(8)
            c.Draw("SAME")
            if i==0:
                leg.AddEntry(c,"TT#1","l")
            if i==2:
                leg.AddEntry(c,"TT#2","l")
        leg.Draw("SAME")




        histos["xy2"].c1.cd()
        for data in mydata:
            c = DrawTrack_xy2(data[0], data[1], data[2], data[3], data[4], data[5], data[6])
            donotdelete.append(c)
            c.Draw("SAME")

        histos["rz"].c1.cd()
        for data in mydata:
            c = DrawTrack_rz(data[0], data[1], data[2], data[3], data[4], data[5], data[6])
            donotdelete.append(c)
            c.Draw("C")

    histos["xy1"].c1.cd()
    #---dev mode---
    histos["xy1"].c1.SaveAs("~/Desktop/fig.root")
    #---
    aux_name = "xy1_tt%i" % options.tt + tag
    if options.save: save(options.outdir, "xy1_tt%i" % options.tt + tag, dot_pdf=False)

    histos["xy2"].c1.cd()
    if options.save: save(options.outdir, "xy2_tt%i" % options.tt + tag, dot_pdf=False)

    histos["rz"].c1.cd()
    if options.save: save(options.outdir, "rz_tt%i" % options.tt + tag, dot_pdf=False)

    return

# ______________________________________________________________________________
def drawer_sitrep(histos, options):
    print "--- SITREP --------------------------------------------------------"
    print


# ______________________________________________________________________________
# Main function
def main(options):

    # Init
    drawer = MyDrawer()
    tchain = TChain("ntupler/tree", "")
    #tchain.AddFileInfoList(options.tfilecoll.GetList())

    # Process
    histos = drawer_book(options)
    drawer_project(tchain, histos, options)
    drawer_draw(histos, options)
    drawer_sitrep(histos, options)

# ______________________________________________________________________________
if __name__ == '__main__':

    # Setup argument parser
    parser = MyParser()
    parser.add_argument("--min-pt", type=float, default=2., help="minimum track pT (default: %(default)s)")
    parser.add_argument("--max-vz", type=float, default=7., help="maximum vertex spread (default: %(default)s)")
    parser.add_argument("--max-rho", type=float, default=110., help="maximum tracker radius (default: %(default)s)")
    parser.add_argument("--tt", type=int, default=27, help="trigger tower (default: %(default)s)")
    parser.add_argument("--file2", type=str, default="", help="secondary file (default: %(default)s)")
    parser.add_argument("--file3", type=str, default="", help="third file (default: %(default)s)")
    parser.add_argument("--save", type=int, default=1, help="save file on disk (default: %(default)s)")
    parser.add_argument("--filetag", type=str, default="", help="filetag (default: %(default)s)")
    parser.add_argument("--tracksfile", type=str, default="", help="filetag (default: %(default)s)")

    options = parser.parse_args()

    find_TTN = re.search("_TT(.+?)_",options.infile)
    TTN = int(find_TTN.group(1)) if find_TTN else -1
    options.tt = TTN

    # raccatta filename in qualche modo
    # Call the main function
    main(options)
