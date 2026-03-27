package dfa

// -------------------------------------------
//SEARCH

type SearchContext struct {
	State *DfaState
	Pos int
	StartPos int
}


func (it *MatchIterator) Current() *MatchResult {
	return it.current
}


func (this *Dfa) Search(substring string) *MatchResult{
	return this.SearchFrom(substring, 0)
}

func (this *Dfa) SearchFrom(text string, start int) *MatchResult{
	for i := start; i < len(text); i++ {
		result := this.matchAt(text, i)
		if result != nil {
			return result
		}
	}
	return nil
}



func (this *Dfa) matchAt(incomingString string, start int) *MatchResult {
	currentState := this.StartState
	lastAcceptPos := -1
	pos := start
	for pos < len(incomingString) {
		ch := incomingString[pos]
		nextState := currentState.Transitions[ch]
		if nextState == nil || nextState == this.ErrorState {break}

		currentState = nextState
		pos++

		if currentState.IsAcceptable {lastAcceptPos = pos} // читаем самое длинное совпадение
	}

	if lastAcceptPos != -1 {
		return &MatchResult{
			Start: start,
			End:   lastAcceptPos,
			Text:  incomingString[start:lastAcceptPos],
		}
	}
	return nil
}

// принятие всей входной строки целиком (не знаю нужен он или нет)
func (this *Dfa) Accepts(incomingString string) bool{
	currentState := this.StartState
	for i := 0; i < len(incomingString); i++ {
		nextState := currentState.Transitions[incomingString[i]]
		if nextState == nil || nextState == this.ErrorState {
			return false
		}
		currentState = nextState
	}
	return currentState.IsAcceptable
}

func (this *Dfa) SearchAll(text string) *MatchIterator {
	return &MatchIterator{
		dfa:  this,
		incomingText: text,
		pos:  0,
	}
}
