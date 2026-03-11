package dfa

import nfa "lab2/pkg/nfa_pkg"

type DfaState struct {
	ID int
	NfaStates map[int]*nfa.NfaState
	Transitions map[byte]*DfaState
	IsAcceptable bool
	GroupInfo map[string]bool
}

type Dfa struct {
	StartState *DfaState
	States []*DfaState
	StateCount int
	Alphabet []byte
	AcceptState []*DfaState
}

func newDfaState(id int, nfastates map[int]*nfa.NfaState) *DfaState {
	return &DfaState{
		ID : id,
		NfaStates : nfastates,
		Transitions : make(map[byte]*DfaState),
		IsAcceptable : false,
		GroupInfo : make(map[string]bool),
	}
}


func BuildDfaFromNfa(n *nfa.Nfa) *Dfa{
	dfa := &Dfa{
		States: make([]*DfaState, 0),
		StateCount : 0,
		Alphabet : make([]byte, 0),
		AcceptState : make([]*DfaState, 0),
	}

}