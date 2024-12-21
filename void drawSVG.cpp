#include <vector>
#include <iostream>
#include <string>
#include <iomanip>
#include <fstream>
#include<cmath>
#include<cstdlib>
using namespace std;
string intToStr(int n);

void drawSVG(ofstream& svgFile,vector<vector<int>> grid){
	int rows=grid.size();
	int cols=grid[0].size();
    vector<string>svg;
    const int grid_edge=25;
    
    int graph_Hedge=grid_edge*(rows+1);
    int graph_Wedge=grid_edge*(cols+1);
  //Header 
  string xml=R"(<svg xmlns=
  "http://www.w3.org/2000/svg" 
  width="graph_Wedge" height="graph_Hedge" viewBox="0 0 graph_Wedge graph_Hedge">
  <rect width="graph_Wedge" height="graph_Hedge" fill="white" />
    )";
  int w;
  int h;
  while(xml.find("Wedge")!=string::npos){
  	w=xml.find("graph_Wedge",0) ;
  	if(w>=0&&w<xml.length()){
  		xml.erase(w,11);
  	    xml.insert(w,intToStr(graph_Wedge));
	  }
  	
  	h=xml.find("graph_Hedge",0);
  	xml.erase(h,11);
  	xml.insert(h,intToStr(graph_Hedge));
  		
  	//xml=xml.replace(h,h+10,intToStr(graph_Hedge));
  }
  svg.push_back(xml);
  svg.push_back(" ");
  //Line
  xml=R"(<g stroke="black" stroke-width="1">)";
  svg.push_back(xml);
  string line_v=R"(<line x1="" y1="0" x2="" y2="graph_Hedge" stroke="black" stroke-width="1" />)";
  string line_h=R"(<line x1="0" y1=" " x2="graph_Wedge" y2=" " stroke="black" stroke-width="1" />)";
  string line_v_thick=R"(<line x1="" y1="0" x2="" y2="graph_Hedge" stroke="black" stroke-width="3" />)";
  string line_h_thick=R"(<line x1="0" y1=" " x2="graph_Wedge" y2=" " stroke="black" stroke-width="3" />)";
  
    w=line_h.find("graph_Wedge",0) ;
    line_h.erase(w,11);
    line_h.insert(w,intToStr(graph_Wedge));
    
	h=line_v.find("graph_Hedge",0);
    line_v.erase(h,11);
    line_v.insert(h,intToStr(graph_Hedge));
  	
  	
    w=line_h_thick.find("graph_Wedge",0) ;
    line_h_thick.erase(w,11);
    line_h_thick.insert(w,intToStr(graph_Wedge));
    
    h=line_v_thick.find("graph_Hedge",0);
    line_v_thick.erase(h,11);
    line_v_thick.insert(h,intToStr(graph_Hedge));
  	
  
  //Vertical Line
  for(int i=0;i<cols+2;i++){
  	 if(i==1||i==cols+1){
  	 	xml=line_v_thick;
	   }else{
	   	xml=line_v;
	   }
    
     int pos=xml.find("x1=",0);
  	 string in=intToStr(grid_edge*i);
  	 xml.insert(pos+4,in);
  	 int end=xml.find("x2=",0);
  	 xml.insert(end+4,in);
  	 svg.push_back(xml);
  	 
  }
  //Horizontal
  for(int i=0;i<rows+2;i++){
     if(i==0||i==cols){
  	 	xml=line_h_thick;
	   }else{
	   	xml=line_h;
	   }
     int pos=xml.find("y1=",0);
  	 string in=intToStr(grid_edge*i);
  	 xml.insert(pos+4,in);
  	 int end=xml.find("y2=",0);
  	 xml.insert(end+4,in);
  	 svg.push_back(xml);
  	 
  }
  
  //Obstacles
 // <rect x="50" y="50" width="50" height="50" fill="gray" />
  
  //<!-- Text Numbers -->
  //Coordinate
  string text=R"(<text x="5" y="" font-size="15" text-anchor="up" fill="blue"></text>)";
  for(int i=0;i<rows;i++){
  	int x=grid_edge*(i+1);
     xml=text;
     int pos=xml.find("y=",0);
  	 string in=intToStr(x);
  	 xml.insert(pos+3,in);
  	 int end=xml.find("><",0);
  	 in=intToStr(rows-1-i);
  	 xml.insert(end+1,in);
  	 svg.push_back(xml);
  	 
  }
  
  text=R"(<text x="" y="" font-size="15" text-anchor="up" fill="blue"></text>)";
  int y=text.find("y",0);
  text.insert(y+3,intToStr(graph_Hedge-1));
  for(int i=0;i<cols;i++){
  	int x=grid_edge*(i+1)+grid_edge/2;
     xml=text;
     int pos=xml.find("x=",0);
  	 string in=intToStr(x);
  	 xml.insert(pos+3,in);
  	 int end=xml.find("><",0);
  	 in=intToStr(i);
  	 xml.insert(end+1,in);
  	 svg.push_back(xml);
  	 
  }
  
  //Grid
  text=R"(<text x="" y="" font-size="10" text-anchor="up" fill="blue"></text>)";
  string temp;
  
  /*int h=graph_Hedge/(rows+1);
  int w=graph_Wedge/(cols+1);*/
  string rect=R"(<rect x="" y="" width="25" height="25" rx="5" ry="5" fill="" stroke="black" stroke-width="1" />)";
  /*int wIndex=rect.find("width",0);
  rect.insert(wIndex+7,intToStr(w));
  int hIndex=rect.find("height",0);
  rect.insert(hIndex+8,intToStr(h));*/
  string store;
  
  for(int i=0;i<rows;i++){
  	temp=text;
  	int pos=temp.find("y=",0);
  	int x=grid_edge*(i+1);
  	string in=intToStr(x);
  	temp.insert(pos+3,in);
  	
  	store=rect;
  	pos=store.find("y=",0);
  	x=i*grid_edge;
  	in=intToStr(x);
  	store.insert(pos+3,in);
  	
  	
  	   for(int j=0;j<cols;j++){
  	     	xml=temp;
  	        int pos=xml.find("x=",0);
  	        int x=(j+1)*grid_edge+grid_edge/2;
  	        string in=intToStr(x);
  	        xml.insert(pos+3,in);
  	        int end=xml.find("><",0);
  	        
  	   	if(grid[i][j]>=0){
			in=intToStr(grid[i][j]);
			xml.insert(end+1,in);
	        svg.push_back(xml);
			 }
			 else{
			 	string t=xml;
			 
			 	
			xml=store;
  	        int pos=xml.find("x=",0);
  	        int x=(j+1)*grid_edge;
  	        string in=intToStr(x);
  	        xml.insert(pos+3,in);
  	       
  	        int fillPlace=xml.find("fill",0)+6;
			 	switch(grid[i][j]){
			 		case -1:
			 			xml.insert(fillPlace,"gray");
			 			break;
			 		case -2:
			 			xml.insert(fillPlace,"yellow");
			 			break;
			 		case -3:
			 			xml.insert(fillPlace,"red");
			 			break;
			 		case -4:
			 			xml.insert(fillPlace,"green");
			 			break;
			 		case -5:
			 			xml.insert(fillPlace,"green");
			 			break;
			 			
			 			
			 	
			 			
				 }
	             svg.push_back(xml);
	             
			 		if(grid[i][j]==-5){
			 		t.insert(end+1,"V");
	                svg.push_back(t);
			 		
				 }
		}
	   }
	   
  }
  
  
  //End
   svg.push_back("</g>");
   svg.push_back("</svg>");
    // Write SVG content to the output stream
    for(int i=0;i<svg.size();i++){
    	svgFile << svg[i]<<endl;
	}	 
	cout<<"Complete SVG drawing!\n";
	
}

