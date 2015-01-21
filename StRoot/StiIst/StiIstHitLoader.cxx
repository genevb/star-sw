#include <iostream>
#include <stdexcept>
#include <cmath>
#include <stdio.h>
#include <stdlib.h>
#include "StEvent/StEnumerations.h"
#include "StMcEvent/StMcEvent.hh"
#include "StEventTypes.h"
#include "StDbUtilities/StGlobalCoordinate.hh"
#include "Sti/StiHit.h"
#include "Sti/StiHitContainer.h"
#include "Sti/StiDetector.h"
#include "Sti/StiDetectorBuilder.h"
#include "Sti/StiTrackContainer.h"
#include "StiIst/StiIstDetectorBuilder.h"
#include "StiIstHitLoader.h"
#include "StMcEvent/StMcTrack.hh"
#include "StMcEvent/StMcIstHit.hh"
#include "StMcEvent/StMcIstHitCollection.hh"
#include "StMcEvent/StMcIstLayerHitCollection.hh"
#include "StIstHitCollection.h"
#include "StIstHit.h"


StiIstHitLoader::StiIstHitLoader() :
   StiHitLoader<StEvent, StiDetectorBuilder>("IstHitLoader")
{}

StiIstHitLoader::StiIstHitLoader(StiHitContainer *hitContainer, Factory<StiHit> *hitFactory,
   StiDetectorBuilder *detector) :
   StiHitLoader<StEvent, StiDetectorBuilder>("IstHitLoader", hitContainer, hitFactory, detector)
{}

StiIstHitLoader::~StiIstHitLoader()
{}

void StiIstHitLoader::loadHits(StEvent *source, Filter<StiTrack> *trackFilter, Filter<StiHit> *hitFilter)
{
   LOG_INFO << "StiIstHitLoader::loadHits(StEvent*) -I- Started" << endm;

   if (!_detector)
      throw runtime_error("StiIstHitLoader::loadHits(StEvent*) - FATAL - _detector==0");

   if (!_hitContainer)
      throw runtime_error("StiIstHitLoader::loadHits(StEvent*) - FATAL - _hitContainer==0");


   StIstHitCollection *col = source->istHitCollection();

   if (!col) {
      LOG_INFO << "StiIstHitLoader::loadHits\tERROR:\tcol==0" << endm;
      LOG_INFO << "You must not have istFastSim in your chain" << endm;
      LOG_INFO << "will return with no action taken" << endm;
      return;
   }

   int nIsthits = col->numberOfHits();
   LOG_DEBUG << "StiIstHitLoader: IST Hits: " << nIsthits << endm;

   if (nIsthits) {
      for (int ladderIdx = 0; ladderIdx < kIstNumLadders; ++ladderIdx ) {
         StIstLadderHitCollection *ladderHitCollection = col->ladder(ladderIdx);

         if (! ladderHitCollection) {cout << "No IST ladder hit collection" << endl; return;}

         for (int sensorIdx = 0; sensorIdx < kIstNumSensorsPerLadder; sensorIdx++)   {
            StIstSensorHitCollection *sensorHitCollection = ladderHitCollection->sensor(sensorIdx);

            if (! sensorHitCollection) {cout << "No IST sensor hit collection" << endl; return;}

            StSPtrVecIstHit &vec = sensorHitCollection->hits();
            LOG_DEBUG << "StiIstHitLoader - collection size: " << vec.size() << " on Ladder " << ladderIdx + 1 << " Sensor " << sensorIdx + 1 << endm;

            for (unsigned int j = 0; j < vec.size(); j++) {
               StIstHit *hit = vec[j];

               if (!hit)
                  throw runtime_error("StiIstHitLoader::loadHits(StEvent*) -E- NULL hit in container");

               if (hit->detector() != kIstId) continue;

               int layer  = 1; //active area
               int ladder = hit->getLadder();
               int sensor = hit->getSensor();

               LOG_DEBUG << *hit << "\n"
                         << *static_cast<StMeasuredPoint*>(hit) << endm;

               const StiDetector *detector = static_cast<StiIstDetectorBuilder*>(_detector)->getActiveDetector(ladder);

               if (!detector)
                  throw runtime_error("StiIstHitLoader::loadHits(StEvent*) -E- NULL detector pointer");

               StiHit *stiHit = _hitFactory->getInstance();

               if (!stiHit) throw runtime_error("StiIstHitLoader::loadHits(StEvent*) -E- stiHit==0");

               stiHit->reset();

               stiHit->setGlobal(detector, hit, hit->position().x(), hit->position().y(), hit->position().z(), hit->charge());
               _hitContainer->add( stiHit );
            } // end hits loop
         } //end sensors loop
      } // end ladders loop
   }// end nIsthits cut

   LOG_INFO << "StiIstHitLoader::loadHits(StEvent*) -I- Done" << endm;
}