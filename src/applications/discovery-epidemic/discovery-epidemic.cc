/* A RapidNet application. Generated by RapidNet compiler. */

#include "discovery-epidemic.h"
#include <cstdlib>
#include "ns3/nstime.h"
#include "ns3/simulator.h"
#include "ns3/type-ids.h"
#include "ns3/rapidnet-types.h"
#include "ns3/rapidnet-utils.h"
#include "ns3/assignor.h"
#include "ns3/selector.h"
#include "ns3/rapidnet-functions.h"

using namespace std;
using namespace ns3;
using namespace ns3::rapidnet;
using namespace ns3::rapidnet::discoveryepidemic;

const string DiscoveryEpidemic::BEACON = "beacon";
const string DiscoveryEpidemic::BEACONLOC = "beaconLoc";
const string DiscoveryEpidemic::ELINKDISCOVERYADD = "eLinkDiscoveryAdd";
const string DiscoveryEpidemic::ELINKDISCOVERYDEL = "eLinkDiscoveryDel";
const string DiscoveryEpidemic::LINK = "link";
const string DiscoveryEpidemic::PERIODIC = "periodic";
const string DiscoveryEpidemic::R1_ECAPERIODIC = "r1_ecaperiodic";

NS_LOG_COMPONENT_DEFINE ("DiscoveryEpidemic");
NS_OBJECT_ENSURE_REGISTERED (DiscoveryEpidemic);

TypeId
DiscoveryEpidemic::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::rapidnet::discoveryepidemic::DiscoveryEpidemic")
    .SetParent<RapidNetApplicationBase> ()
    .AddConstructor<DiscoveryEpidemic> ()
    ;
  return tid;
}

DiscoveryEpidemic::DiscoveryEpidemic()
{
  NS_LOG_FUNCTION_NOARGS ();
}

DiscoveryEpidemic::~DiscoveryEpidemic()
{
  NS_LOG_FUNCTION_NOARGS ();
}

void
DiscoveryEpidemic::DoDispose (void)
{
  NS_LOG_FUNCTION_NOARGS ();

  RapidNetApplicationBase::DoDispose ();
}

void
DiscoveryEpidemic::StartApplication (void)
{
  NS_LOG_FUNCTION_NOARGS ();

  RapidNetApplicationBase::StartApplication ();
  m_event_r1_ecaperiodic=
    Simulator::Schedule (Seconds (1 + (drand48 () * 2)), &DiscoveryEpidemic::R1_ecaperiodic, this);
  RAPIDNET_LOG_INFO("DiscoveryEpidemic Application Started");
}

void
DiscoveryEpidemic::StopApplication ()
{
  NS_LOG_FUNCTION_NOARGS ();

  RapidNetApplicationBase::StopApplication ();
  Simulator::Cancel(m_event_r1_ecaperiodic);
  RAPIDNET_LOG_INFO("DiscoveryEpidemic Application Stopped");
}

void
DiscoveryEpidemic::InitDatabase ()
{
  //RapidNetApplicationBase::InitDatabase ();

  AddRelationWithKeys (LINK, attrdeflist (
    attrdef ("link_attr1", IPV4),
    attrdef ("link_attr2", IPV4)),
    Seconds (5));

}

void
DiscoveryEpidemic::DemuxRecv (Ptr<Tuple> tuple)
{
  RapidNetApplicationBase::DemuxRecv (tuple);

  if (IsRecvEvent (tuple, R1_ECAPERIODIC))
    {
      R1_eca (tuple);
    }
  if (IsRecvEvent (tuple, BEACON))
    {
      R2_eca (tuple);
    }
  if (IsRecvEvent (tuple, BEACONLOC))
    {
      R3_eca (tuple);
    }
  if (IsInsertEvent (tuple, LINK))
    {
      RAddLinkEca0Ins (tuple);
    }
  if (IsRefreshEvent (tuple, LINK))
    {
      RAddLinkEca0Ref (tuple);
    }
  if (IsDeleteEvent (tuple, LINK))
    {
      RDelLinkEca0Del (tuple);
    }
}

void
DiscoveryEpidemic::R1_ecaperiodic ()
{
  RAPIDNET_LOG_INFO ("R1_ecaperiodic triggered");

  SendLocal (tuple (R1_ECAPERIODIC, attrlist (
    attr ("r1_ecaperiodic_attr1", Ipv4Value, GetAddress ()),
    attr ("r1_ecaperiodic_attr2", Int32Value, rand ()))));

  m_event_r1_ecaperiodic = Simulator::Schedule (Seconds(2),
    &DiscoveryEpidemic::R1_ecaperiodic, this);
}

void
DiscoveryEpidemic::R1_eca (Ptr<Tuple> r1_ecaperiodic)
{
  RAPIDNET_LOG_INFO ("R1_eca triggered");

  Ptr<Tuple> result = r1_ecaperiodic;

  result->Assign (Assignor::New ("Broadcast",
    BROADCAST_ADDRESS));

  result = result->Project (
    BEACON,
    strlist ("Broadcast",
      "r1_ecaperiodic_attr1",
      "Broadcast"),
    strlist ("beacon_attr1",
      "beacon_attr2",
      RN_DEST));

  Send (result);
}

void
DiscoveryEpidemic::R2_eca (Ptr<Tuple> beacon)
{
  RAPIDNET_LOG_INFO ("R2_eca triggered");

  Ptr<Tuple> result = beacon;

  result->Assign (Assignor::New ("Local",
    LOCAL_ADDRESS));

  result = result->Project (
    BEACONLOC,
    strlist ("Local",
      "beacon_attr2",
      "Local"),
    strlist ("beaconLoc_attr1",
      "beaconLoc_attr2",
      RN_DEST));

  Send (result);
}

void
DiscoveryEpidemic::R3_eca (Ptr<Tuple> beaconLoc)
{
  RAPIDNET_LOG_INFO ("R3_eca triggered");

  Ptr<Tuple> result = beaconLoc;

  result->Assign (Assignor::New ("Cost",
    ValueExpr::New (Int32Value::New (1))));

  result = result->Project (
    LINK,
    strlist ("beaconLoc_attr1",
      "beaconLoc_attr2",
      "Cost"),
    strlist ("link_attr1",
      "link_attr2",
      "link_attr3"));

  Insert (result);
}

void
DiscoveryEpidemic::RAddLinkEca0Ins (Ptr<Tuple> link)
{
  RAPIDNET_LOG_INFO ("RAddLinkEca0Ins triggered");

  Ptr<Tuple> result = link;

  result = result->Project (
    ELINKDISCOVERYADD,
    strlist ("link_attr1",
      "link_attr2",
      "link_attr3"),
    strlist ("eLinkDiscoveryAdd_attr1",
      "eLinkDiscoveryAdd_attr2",
      "eLinkDiscoveryAdd_attr3"));

  SendLocal (result);
}

void
DiscoveryEpidemic::RAddLinkEca0Ref (Ptr<Tuple> link)
{
  RAPIDNET_LOG_INFO ("RAddLinkEca0Ref triggered");

  Ptr<Tuple> result = link;

  result = result->Project (
    ELINKDISCOVERYADD,
    strlist ("link_attr1",
      "link_attr2",
      "link_attr3"),
    strlist ("eLinkDiscoveryAdd_attr1",
      "eLinkDiscoveryAdd_attr2",
      "eLinkDiscoveryAdd_attr3"));

  SendLocal (result);
}

void
DiscoveryEpidemic::RDelLinkEca0Del (Ptr<Tuple> link)
{
  RAPIDNET_LOG_INFO ("RDelLinkEca0Del triggered");

  Ptr<Tuple> result = link;

  result = result->Project (
    ELINKDISCOVERYDEL,
    strlist ("link_attr1",
      "link_attr2",
      "link_attr3"),
    strlist ("eLinkDiscoveryDel_attr1",
      "eLinkDiscoveryDel_attr2",
      "eLinkDiscoveryDel_attr3"));

  SendLocal (result);
}

