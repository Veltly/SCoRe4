
#include "../include/Logger.hh"
#include <G4ios.hh>

Surface::Logger::Logger(G4String aId, G4int aVerboseLvl) : fId(aId), fVerboseLvl(aVerboseLvl){};

void Surface::Logger::WriteInfo(G4String& aMsg){
	if(fVerboseLvl > 1) {;
	G4cout << " ----Info----> " << fId << ": " << aMsg << "\n";
	}
}

void Surface::Logger::WriteInfo(G4String&& aMsg){
	WriteInfo(aMsg);
}

void Surface::Logger::WriteError(G4String& aMsg){
	if(fVerboseLvl > 0) {	
	G4cout << " ----Error----> " << fId << ": " << aMsg << "\n";
	}
}

void Surface::Logger::WriteError(G4String&& aMsg){
	WriteError(aMsg);
}

void Surface::Logger::WriteDebugInfo(G4String& aMsg){
	if(fVerboseLvl > 2) {	
	G4cout << " ----DebugInfo----> " << fId << ": " << aMsg << "\n";
	}
}

void Surface::Logger::WriteDebugInfo(G4String&& aMsg){
	WriteDebugInfo(aMsg);
}
