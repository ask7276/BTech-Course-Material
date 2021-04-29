#include "ns3/command-line.h"
#include "ns3/config.h"
#include "ns3/flow-monitor.h"
#include "ns3/flow-monitor-module.h"
#include "ns3/internet-stack-helper.h"
#include "ns3/ipv4-address-helper.h"
#include "ns3/ipv4-global-routing-helper.h"
#include "ns3/internet-module.h"
#include "ns3/log.h"
#include "ns3/mobility-helper.h"
#include "ns3/mobility-model.h"
#include "ns3/mobility-module.h"
#include "ns3/on-off-helper.h"
#include "ns3/packet-sink.h"
#include "ns3/packet-sink-helper.h"
#include "ns3/ssid.h"
#include "ns3/string.h"
#include "ns3/tcp-westwood.h"
#include "ns3/wifi-mac.h"
#include "ns3/wifi-module.h"
#include "ns3/yans-wifi-helper.h"
#include <iostream>
#include <cstdlib>
#include <bits/stdc++.h>

NS_LOG_COMPONENT_DEFINE("Lab assignment 4 Group 78");

using namespace std;
using namespace ns3;

int lastTotalRx = 0;
int tcp_seg_size = 1000;
double sim_time_in_sec = 50.0;
Ptr<PacketSink> PS;
bool pcapTrace = true;

int rts_limits[] = {0, 256, 512, 1000};
int dropcount1 = 0, dropcount2 = 0, dropcount320 = 0, dropcount390 = 0;
void MacTxDrop(Ptr<const Packet> p)
{
  NS_LOG_INFO("Packet Drop");
  dropcount1++;
}

void PhyTxDrop(Ptr<const Packet> p)
{
  NS_LOG_INFO("Packet Drop");
  dropcount2++;
}

void PhyRxDrop(Ptr<const Packet> p, WifiPhyRxfailureReason reason)
{
  NS_LOG_INFO("Packet Drop");
  if (p->GetSize() == 90)
    dropcount390++;
  else
    dropcount320++;
}

double func_rts_bandwidth(int rts_seg_size, int rts_num, double sim_time_in_sec)
{
  return (rts_seg_size * rts_num * 8 / sim_time_in_sec) / 1000.0;
}

double func_cts_bandwidth(int cts_seg_size, int cts_num, double sim_time_in_sec)
{
  return (cts_seg_size * cts_num * 8 / sim_time_in_sec) / 1000.0;
}

double func_rts_cts_ack_bandwidth(int rts_cts_ack_seg_size, int rts_num, double sim_time_in_sec)
{
  return (rts_cts_ack_seg_size * rts_num * 8 / sim_time_in_sec) / 1000.0;
}

double func_tcp_ack_bandwidth(int tcp_ack_seg_size, int tcp_ack_num, double sim_time_in_sec)
{
  return (tcp_ack_seg_size * tcp_ack_num * 8 / sim_time_in_sec) / 1000.0;
}

double func_data_bandwidth(int data_size, int tcp_ack_num, double sim_time_in_sec)
{
  return (data_size * tcp_ack_num * 8 / sim_time_in_sec) / 1000.0;
}

void count_stats()
{

  // following sizes are in bytes
  int cts_seg_size = 14;
  int rts_seg_size = 20;
  int rts_cts_ack_seg_size = 14;
  int tcp_ack_seg_size = 74;
  int data_size = tcp_seg_size + 34;

  // count of respective segments
  int rts_cts_ack_num, tcp_ack_num, rts_num, cts_num, data_num;
  rts_cts_ack_num = tcp_ack_num = rts_num = cts_num = data_num = 0;

  FILE *fp = fopen("server.pcap", "r");
  char line[1024];
  while (fgets(line, 1024, fp))
  {
    string s1(line);
    if (s1.find("Acknowledgment") != string::npos)
      rts_cts_ack_num++;
    else if (s1.find("ack") != string::npos and s1.find("seq") == string::npos)
      tcp_ack_num++;
    else if (s1.find("Request-To-Send") != string::npos)
      rts_num++;
    else if (s1.find("Clear-To-Send") != string::npos)
      cts_num++;
    else if (s1.find("seq") != string::npos)
      data_num++;
  }

  // printing the bandwidth used
  printf("(RTS bandwidth,CTS bandwidth,RTSCTSACK bandwidth,TCP ACK bandwidth,TCP segment bandwidth) =\n  (Mbps %f", func_rts_bandwidth(rts_seg_size, rts_num, sim_time_in_sec));
  printf(" %fMbps ", func_cts_bandwidth(cts_seg_size, cts_num, sim_time_in_sec));
  printf(" %fMbps ", func_rts_cts_ack_bandwidth(rts_cts_ack_seg_size, rts_num, sim_time_in_sec));
  printf(" %fMbps ", func_tcp_ack_bandwidth(tcp_ack_seg_size, tcp_ack_num, sim_time_in_sec));
  printf(" %fMbps)\n", func_data_bandwidth(data_size, tcp_ack_num, sim_time_in_sec));
}

void print_the_stats(FlowMonitorHelper &f, Ptr<FlowMonitor> mon)
{
  mon->SerializeToXmlFile("group78.xml", true, true);
  mon->CheckForLostPackets();
  Ptr<Ipv4FlowClassifier> ipv4flowclassifier = DynamicCast<Ipv4FlowClassifier>(f.GetClassifier());
  FlowMonitor::FlowStatsContainer data = mon->GetFlowStats();
  for (map<FlowId, FlowMonitor::FlowStats>::const_iterator i = data.begin(); i != data.end(); i++)
  {
    Ipv4FlowClassifier::FiveTuple t = ipv4flowclassifier->FindFlow(i->first);
    cout << "Flow " << i->first - 1 << " (" << t.sourceAddress << " -> " << t.destinationAddress << ")\n";
    cout << "  (Tx Bytes,TxOffered) = (";
    cout << i->second.txBytes << " ";
    cout << i->second.txBytes * 8.0 / 50.0 / 1000 / 1000 << "Mbps)\n";
    cout << "  Rx Bytes = ("
         << " ";
    cout << i->second.rxBytes << ")\n";
    cout << "  Throughput = " << i->second.rxBytes * 8.0 / 50.0 / 1000 / 1000 << " Mbps\n";
  }

  double averageThroughput = ((PS->GetTotalRx() * 8) / (1e6 * sim_time_in_sec));
  cout << "\nAverage throughtput = " << averageThroughput << " Mbps"
       << "\n";
  
}

void progress(int rts_ctc_threshold)
{

  // setting the appropriate parameters
  Config::SetDefault("ns3::WifiRemoteStationManager::RtsCtsThreshold", UintegerValue(rts_ctc_threshold));
  Config::SetDefault("ns3::WifiRemoteStationManager::FragmentationThreshold", StringValue("1500"));
  Config::SetDefault("ns3::TcpSocket::SegmentSize", UintegerValue(tcp_seg_size));

  // Setup for proper channel and layer
  WifiHelper wifi;
  YansWifiChannelHelper wifiChannel;
  WifiMacHelper mac_helper;
  YansWifiPhyHelper wifi_physical_layer = YansWifiPhyHelper::Default();

  wifi.SetStandard(WIFI_PHY_STANDARD_80211n_5GHZ);
  wifiChannel.SetPropagationDelay("ns3::ConstantSpeedPropagationDelayModel");
  wifiChannel.AddPropagationLoss("ns3::RandomPropagationLossModel");
  wifi_physical_layer.SetChannel(wifiChannel.Create());
  wifi_physical_layer.SetErrorRateModel("ns3::YansErrorRateModel");
  wifi.SetRemoteStationManager("ns3::ConstantRateWifiManager", "DataMode", StringValue("HtMcs7"), "ControlMode", StringValue("HtMcs0"));
  mac_helper.SetType("ns3::AdhocWifiMac");

  Ipv4AddressHelper adr;
  adr.SetBase("10.0.0.0", "255.255.255.0");
  Ipv4InterfaceContainer interface_1, interface_2;

  // Create 3 Nodes
  NodeContainer sample_node1, sample_node2, sample_node3, cli_nds;
  sample_node1.Create(1);
  sample_node2.Create(1);
  sample_node3.Create(1);
  // installing server and client nodes
  NetDeviceContainer server, client;
  Ptr<Node> serv_nds = sample_node2.Get(0);
  server = wifi.Install(wifi_physical_layer, mac_helper, serv_nds);
  cli_nds = NodeContainer(sample_node1.Get(0), sample_node3.Get(0));
  client = wifi.Install(wifi_physical_layer, mac_helper, cli_nds);

  // assigning positions to the nodes created using .create method
  MobilityHelper helper;
  InternetStackHelper helper_2;
  Ptr<ListPositionAllocator> locations = CreateObject<ListPositionAllocator>();
  locations->Add(Vector(0.0, 0.0, 0.0));   // first node position
  locations->Add(Vector(250.0, 0.0, 0.0)); // second node position
  locations->Add(Vector(500.0, 0.0, 0.0)); // third node position
  helper.SetPositionAllocator(locations);
  helper.SetMobilityModel("ns3::ConstantPositionMobilityModel");

  helper.Install(sample_node1.Get(0)); // install nodes
  helper_2.Install(sample_node1.Get(0));

  helper.Install(sample_node2.Get(0));
  helper_2.Install(sample_node2.Get(0));

  helper.Install(sample_node3.Get(0));
  helper_2.Install(sample_node3.Get(0));

  interface_1 = adr.Assign(server);
  interface_2 = adr.Assign(client);

  // Install TCP Receiver on the access point
  string protocol = "ns3::TcpSocketFactory";
  PacketSinkHelper receiver(protocol, InetSocketAddress(Ipv4Address::GetAny(), 9));
  ApplicationContainer server_node = receiver.Install(serv_nds);
  OnOffHelper transmitter(protocol, (InetSocketAddress(interface_1.GetAddress(0), 9)));
  transmitter.SetAttribute("PacketSize", UintegerValue(tcp_seg_size));
  transmitter.SetAttribute("DataRate", DataRateValue(DataRate("300kbps")));
  transmitter.SetAttribute("OnTime", StringValue("ns3::ConstantRandomVariable[Constant=1]"));
  transmitter.SetAttribute("OffTime", StringValue("ns3::ConstantRandomVariable[Constant=0]"));
  ApplicationContainer client_nodes = transmitter.Install(cli_nds);

  PS = StaticCast<PacketSink>(server_node.Get(0));

  // Start Applications
  int x = rand() % 5 + 1;
  double y = (double)x * 1.0;
  server_node.Start(Seconds(y));
  client_nodes.Start(Seconds(y));

  if (pcapTrace)
  {
    wifi_physical_layer.SetPcapDataLinkType(WifiPhyHelper::DLT_IEEE802_11_RADIO);
    wifi_physical_layer.EnablePcap("Server", server);
    wifi_physical_layer.EnablePcap("Station", client);
  }

  // FLow Monitor
  FlowMonitorHelper f;
  Ptr<FlowMonitor> mon = f.InstallAll();
  Config::ConnectWithoutContext("/NodeList/*/DeviceList/*/$ns3::WifiNetDevice/Mac/MacTxDrop", ns3::MakeCallback(&MacTxDrop));
  Config::ConnectWithoutContext("/NodeList/*/DeviceList/*/$ns3::WifiNetDevice/Phy/PhyRxDrop", ns3::MakeCallback(&PhyRxDrop));
  Config::ConnectWithoutContext("/NodeList/*/DeviceList/*/$ns3::WifiNetDevice/Phy/PhyTxDrop", MakeCallback(&PhyTxDrop));
  Simulator::Stop(Seconds(sim_time_in_sec + y + 1));
  Simulator::Run();
  Simulator::Destroy();
  auto print=[&](void){
    print_the_stats(f,mon);
    count_stats();
  };
  print();
}

int main()
{

  for (int i = 0; i < 4; i++)
  {
    int rts_ctc_threshold = rts_limits[i];
    cout << "Statistics for the RTS Threshold : " << rts_ctc_threshold << "\n";
    progress(rts_ctc_threshold);
    system("tcpdump -nn -tt -r Server-1-0.pcap > server.pcap");
    cout << "Bandwidth wasted " << (20 * dropcount320 * 8 + 90 * dropcount390 * 8) / (1e6 * 50) << " mbps\n\n";
    dropcount320 = dropcount390 = 0;
  }
  return 0;
}
