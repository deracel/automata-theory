package dfa

import (
	"fmt"
	"strings"
)


type RegexBuilder struct {
	d *Dfa
	states []*DfaState
	stateIndex map[*DfaState]int
	indexState map[int]*DfaState
	n int
}

func NewRegexBuilder(d *Dfa) *RegexBuilder {
	states := make([]*DfaState, 0)
	for _, state := range d.States {
		if state != d.ErrorState{
			states = append(states, state)
		}
	}
	/*
	sort.Slice(states, func(i, j int) bool {
		return states[i].ID < states[j].ID
	})

	 */
	stateIndex := make(map[*DfaState]int)
	indexState := make(map[int]*DfaState)
	count := 1
	for _, state := range states {
		stateIndex[state] = count
		indexState[count] = state
		count++
	}
	return &RegexBuilder{
		d:          d,
		states:     states,
		stateIndex: stateIndex,
		indexState: indexState,
		n: len(states),
	}
}


func (this *RegexBuilder) BuildRegex() (string, error) {
	if this.n == 0{
		return "", fmt.Errorf("Error: states count = 0\n")
	}
	R := make([][][]string, this.n+1)
	for i := 1; i <= this.n; i++ {
		R[i] = make([][]string, this.n+1)
		for j := 1; j <= this.n; j++ {
			R[i][j] = make([]string, this.n+1)
		}
	}

	for i := 1; i <= this.n; i++{
		for j := 1; j <= this.n; j++{
			R[i][j][0] = this.calculateBasis(i, j)
		}
	}

	for k := 1; k <= this.n; k++ {
		for i := 1; i <= this.n; i++ {
			for j := 1; j <= this.n; j++ {
				prt1 := R[i][j][k-1]
				prt2 := R[i][k][k-1]
				prt3Star := R[k][k][k-1]
				prt4 := R[k][j][k-1]

				R[i][j][k] = this.makeUnion(
					prt1,
					this.makeConcat(
						prt2,
						this.makeConcat(
							this.makeKlini(prt3Star),
							prt4),
						),
					)

			}
		}
		this.print(R, k)
	}

	startInd := this.stateIndex[this.d.StartState]
	var result string

	for _, acceptSt := range this.d.AcceptStates {
		acceptInd := this.stateIndex[acceptSt]
		expression := R[startInd][acceptInd][this.n]
		result = this.makeUnion(result, expression)
	}
	result = this.simplify(result)
	return result, nil
}

func (this *RegexBuilder) calculateBasis(i, j int) string {
	stateI := this.indexState[i]

	var result string
	if i == j {
		result = "ε"
	}
	for _, symbol := range this.d.Alphabet{
		nextState := stateI.Transitions[symbol]
		if nextState != nil && nextState != this.d.ErrorState{
			if nextIndx, ok := this.stateIndex[nextState]; ok && nextIndx == j {
				token := escapeSymbol(symbol)
				result = this.makeUnion(result, token)
			}
		}
	}
	if result == ""{
		return "∅"
	}
	return result
}

func (this *RegexBuilder) print(R [][][]string, k int) {
	fmt.Printf("=== k=%d ===\n", k)
	for i := 1; i <= this.n; i++ {
		for j := 1; j <= this.n; j++ {
			if R[i][j][k] != "" && R[i][j][k] != "∅" {
				fmt.Printf("R[%d][%d][%d] = %s\n", i, j, k, R[i][j][k])
			}
		}
	}
}

func (this *RegexBuilder) makeUnion(str1 string, str2 string) string {
	if str1 == "" || str1 == "∅"{
		return str2
	}
	if str2 == "" || str2 == "∅"{
		return str1
	}
	if str1 == str2 {
		return str1
	}

	if str1 == "ε" && this.containsEpsilon(str2) {
		return str2
	}
	if str2 == "ε" && this.containsEpsilon(str1) {
		return str1
	}

	str1 = this.addPars(str1)
	str2 = this.addPars(str2)

	return str1 + "|" + str2
}

func (this *RegexBuilder) makeConcat(str1, str2 string) string {
	if str1 == "" || str1 == "∅" || str2 == "" || str2 == "∅"{
		return "∅"
	}
	if str1 == "ε"{
		return str2
	}
	if str2 == "ε"{
		return str1
	}

	str1 = this.addPars(str1)
	str2 = this.addPars(str2)
	return "(" + str1 + ")" + "(" + str2 + ")"
}





func (this *RegexBuilder) makeKlini(str string) string {
	if str == "" || str == "∅" {
		return "ε"
	}
	if str == "ε"{
		return "ε"
	}

	if strings.HasPrefix(str, "(") && strings.HasSuffix(str, ")..."){
		return str
	}
	str = this.addPars(str)
	return str + "..."
}


func (this *RegexBuilder) addPars(str string) string {
	if strings.HasPrefix(str, "(") && strings.HasSuffix(str, ")") {
		return str
	}
	if len(str) == 1 || str == "ε" {
		return str
	}
	if strings.HasSuffix(str, "...") {
		return str
	}
	if strings.Contains(str, "|") {
		return "(" + str + ")"
	}
	return str
}

func escapeSymbol(symbol byte) string {
	switch symbol {
	case '|', '.', '?', '(', ')', '{', '}', '<', '>', '%':
		return "%" + string(symbol) + "%"
	default:
		return string(symbol)
	}
}


func (this *RegexBuilder) containsEpsilon(s string) bool {
	if s == "ε" {
		return true
	}
	if strings.Contains(s, "|") {
		parts := strings.Split(s, "|")
		for _, p := range parts {
			if p == "ε" {
				return true
			}
		}
	}
	return false
}

func (this *RegexBuilder) simplify(expr string) string {
	if strings.Contains(expr, "|") {
		parts := strings.Split(expr, "|")
		clean := make([]string, 0, len(parts))
		for _, p := range parts {
			if p != "∅" {
				clean = append(clean, p)
			}
		}
		if len(clean) == 1 {
			expr = clean[0]
		} else if len(clean) > 1 {
			expr = strings.Join(clean, "|")
		} else {
			expr = "∅"
		}
	}

	// Убираем ε из конкатенаций
	expr = this.removeEpsilons(expr)

	return expr
}

// NormalizeRegex удаляет все символы ε из регулярного выражения
func (this *RegexBuilder) removeEpsilons(regex string) string {
	if regex == "" {
		return ""
	}
	result := strings.ReplaceAll(regex, "ε", "")
	return result
}

func (rb *RegexBuilder) removeEpsilonFromConcat(expr string) string {
	if !strings.Contains(expr, ".") && !strings.Contains(expr, "(") {
		return expr
	}
	return expr
}


func BuildRegexFromDFA(d *Dfa) (string, error) {
	builder := NewRegexBuilder(d)
	return builder.BuildRegex()
}