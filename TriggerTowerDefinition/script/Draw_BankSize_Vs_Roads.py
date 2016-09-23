from numpy import genfromtxt
from ROOT import TLatex, TGraph, TCanvas, TLegend
from cebefo_style import *

MarkerStyle=[20,21,22,29,33]
EmptyMarkerStyle=[24,25,26,30,27]

ColorList=[2,4,8,6,1]
LightColorList=[46,38,29,40,15]

donotdelete = []

def main():
    # Creating the canvas
    cebefo_style()

    canvas = TCanvas("canvas","canvas",800,600)
    canvas.SetLogy()
    canvas.SetLeftMargin(0.097)
    canvas.SetRightMargin(0.03132832)

    donotdelete.append(canvas)
    gr_overall = TGraph(1)
    gr_overall.SetTitle("Comparison between different super-strips configurations")
    gr_overall.SetPoint(1,100,1e6)
    gr_overall.SetMarkerStyle(6)
    gr_overall.SetMarkerColor(0)

    gr_overall.GetYaxis().SetTitle("Bank size")
    gr_overall.GetYaxis().SetTitleOffset(1.)
    gr_overall.GetYaxis().SetRangeUser(0.5*1e6,3e7)

    gr_overall.GetXaxis().SetTitle("# Roads per event 0.95 CL")
    gr_overall.GetXaxis().SetLimits(60,200)

    gr_overall.Draw("AP")

    donotdelete.append(gr_overall)

    xleg =[0.62,0.96]
    yleg =[0.70,0.92]
    # Drawing new TT def performance----------------------------------------------------------------
    mydata = genfromtxt("_txt/13_AM_Sim_fast_stat.txt",skip_header=0,comments="#",delimiter='\t', dtype=None)
    leg = TLegend(xleg[0],yleg[0],xleg[1],yleg[1])
    # leg.SetLineWidth(0)
    for i,data in enumerate(mydata):
        EvType = data[0]
        PU = data[1]
        SSConf = data[2]
        BankSize = data[3]
        RoadsMean = data[4]
        Roads95Cl = data[5]

        if(i==0):
            leg.SetHeader("New TT: "+EvType+" PU"+str(PU))
            header = leg.GetListOfPrimitives().First()
            header.SetTextAlign(22)

        gr = TGraph(1)
        gr.SetPoint(1,Roads95Cl,BankSize)
        # print Roads95Cl,BankSize
        color = 0
        if "nz4" in SSConf:
            color = ColorList[0]
        elif "nz6" in SSConf:
            color = ColorList[1]
        elif "nz8" in SSConf:
            color = ColorList[2]
        else:
            color = ColorList[-1]

        gr.SetMarkerColor(color)

        style = 0
        if "L5x2_L10x2" in SSConf:
            style = MarkerStyle[0]
        elif "L5x2" in SSConf:
            style = MarkerStyle[1]
        else:
            style = MarkerStyle[-1]

        gr.SetMarkerStyle(style)

        entry_label = SSConf
        leg.AddEntry(gr,entry_label,"p")

        note = TLatex(3+Roads95Cl, BankSize,SSConf)
        note.SetTextSize(0.03)
        gr.GetListOfFunctions().Add(note)

        gr.Draw("P");
        donotdelete.append(gr)

    leg.Draw("SAME")
    donotdelete.append(leg)


    # Drawing the old definition performance-------------------------------------------------------
    mydata = genfromtxt("_txt/11_AM_Sim_fast_stat_OldTTDef.txt",skip_header=0,comments="#",delimiter='\t', dtype=None)
    leg = TLegend(xleg[0],yleg[0]-0.2,xleg[1],yleg[0])
    # leg.SetLineWidth(0)
    for i,data in enumerate(mydata):
        EvType = data[0]
        PU = data[1]
        SSConf = data[2]
        BankSize = data[3]
        RoadsMean = data[4]
        Roads95Cl = data[5]

        if(i==0):
            leg.SetHeader("Old TT: "+EvType+" PU"+str(PU))
            header = leg.GetListOfPrimitives().First()
            header.SetTextAlign(22)

        gr = TGraph(1)
        gr.SetPoint(1,Roads95Cl,BankSize)
        # print Roads95Cl,BankSize
        color = 0
        if "nz4" in SSConf:
            color = LightColorList[0]
        elif "nz6" in SSConf:
            color = LightColorList[1]
        elif "nz8" in SSConf:
            color = LightColorList[2]
        else:
            color = LightColorList[-1]

        gr.SetMarkerColor(color)

        style = 0
        if "L5x2_L10x2" in SSConf:
            style = EmptyMarkerStyle[0]
        elif "L5x2" in SSConf:
            style = EmptyMarkerStyle[1]
        else:
            style = EmptyMarkerStyle[-1]

        gr.SetMarkerStyle(style)

        entry_label = SSConf
        leg.AddEntry(gr,entry_label,"p")

        # note = TLatex(5+Roads95Cl, BankSize,SSConf)
        # note.SetTextSize(0.03)
        # gr.GetListOfFunctions().Add(note)

        gr.Draw("P");
        donotdelete.append(gr)

    leg.Draw("SAME")
    donotdelete.append(leg)


    canvas.SaveAs("_fig/13_BankSize_Vs_Roads95CL.pdf")


if __name__ == '__main__':

    # Call the main function
    main()
