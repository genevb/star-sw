// $Id: AliHLTTPCCASlicesLinksPerformance.cxx,v 1.2 2012/08/13 19:35:05 fisyak Exp $
// **************************************************************************
// This file is property of and copyright by the ALICE HLT Project          *
// ALICE Experiment at CERN, All rights reserved.                           *
//                                                                          *
// Primary Authors: Sergey Gorbunov <sergey.gorbunov@kip.uni-heidelberg.de> *
//                  Ivan Kisel <kisel@kip.uni-heidelberg.de>                *
//                  for The ALICE HLT Project.                              *
//                                                                          *
// Developed by:   Igor Kulakov <I.Kulakov@gsi.de>                          *
//                 Maksym Zyzak <M.Zyzak@gsi.de>                            *
//                                                                          *
// Permission to use, copy, modify and distribute this software and its     *
// documentation strictly for non-commercial purposes is hereby granted     *
// without fee, provided that the above copyright notice appears in all     *
// copies and that both the copyright notice and this permission notice     *
// appear in the supporting documentation. The authors make no claims       *
// about the suitability of this software for any purpose. It is            *
// provided "as is" without express or implied warranty.                    *
//                                                                          *
//***************************************************************************

#include "AliHLTTPCCAPerformanceBase.h"
#include "AliHLTTPCCASliceLinksPerformance.h"
#include "AliHLTTPCCASlicesPerformance.h"
#include "AliHLTTPCCASlicesLinksPerformance.h"

#include "AliHLTArray.h"
#include "AliHLTTPCCAMCTrack.h"
#include "AliHLTTPCCAMCPoint.h"

void AliHLTTPCCASlicesLinksPerformance::SetNewEvent(const AliHLTTPCCAGBTracker * const Tracker,
                            AliHLTResizableArray<AliHLTTPCCAHitLabel> *hitLabels,
                            AliHLTResizableArray<AliHLTTPCCAMCTrack> *mcTracks,
                            AliHLTResizableArray<AliHLTTPCCALocalMCPoint> *localMCPoints)
{
  AliHLTTPCCAPerformanceBase::SetNewEvent(Tracker, hitLabels, mcTracks, localMCPoints);

    /// Init subperformances
  if (fFirstCall){
    slicePerformances.resize(fTracker->NSlices());
    for (unsigned int iPerf = 0; iPerf < slicePerformances.size(); iPerf++){
      slicePerformances[iPerf] = new AliHLTTPCCASliceLinksPerformance(iPerf);
    }
  }
  fFirstCall = false;
  
  for (unsigned int iPerf = 0; iPerf < slicePerformances.size(); iPerf++){
    slicePerformances[iPerf]->SetNewEvent(fTracker, fHitLabels, fMCTracks, fLocalMCPoints);
  }

} // void AliHLTTPCCASlicesPerformance::SetNewEvent