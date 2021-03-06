// This file is part of GQCG-gqcp.
// 
// Copyright (C) 2017-2019  the GQCG developers
// 
// GQCG-gqcp is free software: you can redistribute it and/or modify
// it under the terms of the GNU Lesser General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
// 
// GQCG-gqcp is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser General Public License for more details.
// 
// You should have received a copy of the GNU Lesser General Public License
// along with GQCG-gqcp.  If not, see <http://www.gnu.org/licenses/>.
// 
#pragma once


#include "Mathematical/Representation/Matrix.hpp"


namespace GQCP {


/**
 *  A class that represents an spin-unresolved ONV (occupation number vector)

 *  An spin-unresolved ONV in quantum chemistry is a string of creation operators acting on top of a vacuum state.
 *  An example for 3 alpha electrons in a spin-unresolved ONV basis spanned by 4 spatial orbitals is
 *      a_1^\dagger a_2^\dagger a_3^\dagger |vac> = |1,1,1,0>
 *
 *  In this code bitstrings are read from right to left. This means that the least significant bit relates to the first orbital.
 *  Using this notation is how normally bits are read, leading to more efficient code.
 *  As is also usual, the least significant bit has index 0. The previous example is then represented by the bit string "0111" (7).
 */
class SpinUnresolvedONV {
private:
    size_t K;  // number of spatial orbitals
    size_t N;  // number of electrons
    size_t unsigned_representation;
    VectorXs occupation_indices;  // the occupied orbital electron indices
                                  // it is a vector of N elements in which occupation_indices[j]
                                  // gives the occupied orbital index for electron j


public:
    // CONSTRUCTORS
    /**
     *  @param K                        the number of orbitals
     *  @param N                        the number of electrons
     *  @param unsigned_representation  the representation for the spin-unresolved ONV as an unsigned integer
     */
    SpinUnresolvedONV(size_t K, size_t N, size_t unsigned_representation);

    /**
     *  Constructs a default spin-unresolved ONV without a representation
     *
     *  @param K                        the number of orbitals
     *  @param N                        the number of electrons
     */
    SpinUnresolvedONV(size_t K, size_t N);


    // OPERATORS
    /**
     *  @param os       the output stream which the spin-unresolved ONV should be concatenated to
     *  @param onv      the spin-unresolved ONV that should be concatenated to the output stream
     *
     *  @return the updated output stream
     */
    friend std::ostream& operator<<(std::ostream& os, const SpinUnresolvedONV& onv);

    /**
     *  @param other    the other spin-unresolved ONV
     *
     *  @return if this spin-unresolved ONV is the same as the other spin-unresolved ONV
     */
    bool operator==(SpinUnresolvedONV& other) const;

    /**
     *  @param other    the other spin-unresolved ONV
     *
     *  @return if this spin-unresolved ONV is not the same as the other spin-unresolved ONV
     */
    bool operator!=(SpinUnresolvedONV& other) const;


    // SETTERS
    /**
     *  @param unsigned_representation      the new representation as an unsigned integer
     *
     *  Set the representation of an spin-unresolved ONV to a new representation and call update the occupation indices accordingly
     */
    void set_representation(size_t unsigned_representation);


    // GETTERS
    size_t get_unsigned_representation() const { return unsigned_representation; }
    const VectorXs& get_occupation_indices() const { return occupation_indices; }

    /**
     *  @param electron_index       the index of the electron
     *
     *  @return the index of the orbital that the electron occupies. For the bitset "100", this would be 2: the conversion from right-to-left is already made
     */
    size_t get_occupation_index(size_t electron_index) const { return occupation_indices(electron_index); }


    // PUBLIC METHODS
    /**
     *  Extracts the positions of the set bits from the this->unsigned_representation and places them in the this->occupation_indices
     */
    void updateOccupationIndices();

    /**
     *  @param p    the orbital index starting from 0, counted from right to left
     *
     *  @return if the p-th spatial orbital is occupied
     */
    bool isOccupied(size_t p) const;

    /**
     *  @param indices      the orbital indices (starting from 0)
     *
     *  @return if all given indices are occupied
     */
    bool areOccupied(const std::vector<size_t>& indices) const;

    /**
     *  @param p    the orbital index starting from 0, counted from right to left
     *
     *  @return if the p-th spatial orbital is not occupied
     */
    bool isUnoccupied(size_t p) const;

    /**
     *  @param indices      the orbital indices (starting from 0)
     *
     *  @return if all the given indices are unoccupied
     */
    bool areUnoccupied(const std::vector<size_t>& indices) const;

    /**
     *  @param index_start      the starting index (included), read from right to left
     *  @param index_end        the ending index (not included), read from right to left
     *
     *  @return the representation of a slice (i.e. a subset) of the spin string (read from right to left) between index_start (included) and index_end (not included)
     *
     *      Example:
     *          "010011".slice(1, 4) => "01[001]1" -> "001"
     */
    size_t slice(size_t index_start, size_t index_end) const;

    /**
     *  @param p        the orbital index starting from 0, counted from right to left
     *
     *  @return the phase factor (+1 or -1) that arises by applying an annihilation or creation operator on orbital p
     *
     *  Let's say that there are m electrons in the orbitals up to p (not included). If m is even, the phase factor is (+1) and if m is odd, the phase factor is (-1), since electrons are fermions
     */
    int operatorPhaseFactor(size_t p) const;

    /**
     *  @param p    the orbital index starting from 0, counted from right to left
     *
     *  @return if we can apply the annihilation operator (i.e. 1->0) for the p-th spatial orbital. Subsequently perform an in-place annihilation on the orbital p
     *
     *  IMPORTANT: does not update the occupation indices for performance reasons, if required call updateOccupationIndices()!
     */
    bool annihilate(size_t p);

    /**
     *  @param indices      the orbital indices (starting from 0)
     *
     *  @return if we can apply all annihilation operators (i.e. 1->0) on the given indices. If possible, subsequently perform in-place annihilations on all the given indices
     *
     *  IMPORTANT: does not update the occupation indices for performance reasons, if required call updateOccupationIndices()!
     */
    bool annihilateAll(const std::vector<size_t>& indices);

    /**
     *  @param p        the orbital index starting from 0, counted from right to left
     *  @param sign     the current sign of the operator string
     *
     *  @return if we can apply the annihilation operator (i.e. 1->0) for the p-th spatial orbital. Subsequently perform an in-place annihilation on the orbital p. Furthermore, update the sign according to the sign change (+1 or -1) of the spin string after annihilation
     *
     *  IMPORTANT: does not update the occupation indices for performance reasons, if required call updateOccupationIndices()!
     */
    bool annihilate(size_t p, int& sign);

    /**
     *  @param indices      the indices of the orbitals that should be annihilated (the first index is annihilated first)
     *  @param sign     the current sign of the operator string
     *
     *  @return if we can apply all annihilation operators (i.e. 1->0) on the given indices. Subsequently perform in-place annihilations on the given indices. Furthermore, update the sign according to the sign change (+1 or -1) of the spin string after the annihilations.
     *
     *  IMPORTANT: does not update the occupation indices for performance reasons, if required call updateOccupationIndices()!
     */
    bool annihilateAll(const std::vector<size_t>& indices, int& sign);

    /**
     *  @param p        the orbital index starting from 0, counted from right to left
     *  @return if we can apply the creation operator (i.e. 0->1) for the p-th spatial orbital. Subsequently perform an in-place creation on the orbital p
     *
     *  IMPORTANT: does not update the occupation indices for performance reasons, if required call updateOccupationIndices()!
     */
    bool create(size_t p);

    /**
     *  @param p        the orbital index starting from 0, counted from right to left
     *  @param sign     the current sign of the operator string
     *
     *  @return if we can apply the creation operator (i.e. 0->1) for the p-th spatial orbital. Subsequently perform an in-place creation on the orbital p. Furthermore, update the sign according to the sign change (+1 or -1) of the spin string after creation.
     *
     *  IMPORTANT: does not update the occupation indices for performance reasons, if required call updateOccupationIndices()!
     */
    bool create(size_t p, int& sign);

    /**
     *  @param indices      the indices of the orbitals that should be created
     *
     *  @return if we can apply all creation operators (i.e. 0->1) on the given indices. Subsequently perform in-place creations on the given indices
     *
     *  IMPORTANT: does not update the occupation indices for performance reasons, if required call updateOccupationIndices()!
     */
    bool createAll(const std::vector<size_t>& indices);

    /**
     *  @param indices      the indices of the orbitals that should be annihilated (the first index is annihilated first)
     *  @param sign         the current sign of the operator string
     *
     *  @return if we can apply all annihilation operators (i.e. 1->0) on the given indices. Subsequently perform in-place annihilations on the given indices. Furthermore, update the sign according to the sign change (+1 or -1) of the spin string after the annihilations.
     *
     *  IMPORTANT: does not update the occupation indices for performance reasons, if required call updateOccupationIndices()!
     */
    bool createAll(const std::vector<size_t>& indices, int& sign);

    /**
     *  @param other        the other spin-unresolved ONV
     *
     *  @return the number of different occupations between this spin-unresolved ONV and the other, i.e. two times the number of electron excitations
     */
    size_t countNumberOfDifferences(const SpinUnresolvedONV& other) const;

    /**
     *  @param other        the other spin-unresolved ONV
     *
     *  @return the indices of the orbitals (from right to left) that are occupied in this spin-unresolved ONV, but unoccupied in the other
     */
    std::vector<size_t> findDifferentOccupations(const SpinUnresolvedONV &other) const;

    /**
     *  @param other        the other spin-unresolved ONV
     *
     *  @return the indices of the orbitals (from right to left) that are occupied both this spin-unresolved ONV and the other
     */
    std::vector<size_t> findMatchingOccupations(const SpinUnresolvedONV& other) const;

    /**
     *  @return a string representation of the spin-unresolved ONV
     */
    std::string asString() const;
};


}  // namespace GQCP
